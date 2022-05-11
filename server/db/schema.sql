create table users
(
    id           varchar not null
        constraint user_pk
            primary key,
    display_name varchar default 'Guest':: character varying
);

create table sessions
(
    id            uuid    not null
        constraint session_pk
            primary key,
    game_id       varchar not null,
    invite_code   varchar not null,
    host_id       uuid    null,
    game_settings json,
    constraint fk_host foreign key (host_id) references users (id)
);

create table statuses
(
    session_id uuid not null
        constraint status_session_id_fk
            references sessions
            on delete cascade,
    status     integer,
    saved_on   timestamp default current_timestamp
);

create unique index session_id_uindex
    on sessions (id);

create unique index session_invite_code_uindex
    on sessions (invite_code);

create table players
(
    session_id  uuid    not null
        constraint player_session_id_fk
            references sessions
            on delete cascade,
    user_id     varchar not null
        constraint player_user_id_fk
            references users
            on delete restrict,
    player_id   integer not null,
    score       integer,
    playerstate text    not null,
    constraint player_pk
        primary key (session_id, user_id),
    constraint uq_playerid_sessionid
        unique (session_id, player_id)
);

alter table sessions
    add constraint sessions_players_session_id_user_id_fk
        foreign key (id, host_id) references players (session_id, user_id);

create table globalstates
(
    session_id  uuid                    not null
        constraint globalstates_pk
            primary key
        constraint globalstates_sessions_id_fk
            references sessions
            on delete cascade,
    globalstate text,
    is_terminal boolean   default false not null,
    saved_on    timestamp default current_timestamp
);

create
    or replace function leave_session(m_session_id uuid, m_user_id uuid) returns void as
$$
declare
begin
    if
            (select host_id
             from sessions
             where id = m_session_id) = m_user_id then
        if (select user_id
            from players
            where session_id = m_session_id
              and user_id != m_user_id
            limit 1) IS NOT NULL then
            update sessions
            set host_id = (select user_id from players where session_id = m_session_id and user_id != m_user_id limit 1)
            where id = m_session_id;
        else
            delete
            from sessions
            where id = m_session_id;
        end if;
    end if;
    delete
    from players
    where user_id = m_user_id
      and session_id = m_session_id;
end
$$
    language plpgsql;

-- STAISTICS

create or replace function get_sessions_info(game_id_ varchar)
    returns table
            (
                session_id uuid,
                duration   numeric,
                max_status integer
            )
as
$$
declare
begin
    -- noinspection SqlAggregates

    RETURN QUERY SELECT statuses.session_id,
                        EXTRACT(epoch from (MAX(statuses.saved_on) - MIN(statuses.saved_on))) as duration,
                        MAX(statuses.status)                                                  as max_status
                 FROM (statuses inner join sessions s on s.id = statuses.session_id)
                 WHERE s.game_id = game_id_
                   and (statuses.status = 1 or statuses.status = 2)
                 GROUP BY statuses.session_id
                 HAVING MAX(statuses.status) >= 2;
end;
$$
    language plpgsql;

create or replace function get_average_session_time_sec(game_id_ varchar) returns int as
$$
declare
begin
    RETURN (SELECT AVG(durations.duration)
            FROM get_sessions_info(game_id_) as durations);
end;
$$
    language plpgsql;

create or replace function get_total_time_sec(game_id_ varchar) returns int as
$$
declare
begin
    RETURN (SELECT COALESCE(SUM(durations.duration), 0)
            FROM get_sessions_info(game_id_) as durations);
end;
$$
    language plpgsql;

create or replace function get_sessions_num(game_id_ varchar) returns int as
$$
declare
begin
    RETURN (SELECT COUNT(durations.duration)
            FROM get_sessions_info(game_id_) as durations);
end;
$$
    language plpgsql;

create or replace function get_average_players_num(game_id_ varchar) returns int as
$$
declare
begin
    RETURN (SELECT AVG(players_nums.players_num)
            FROM (SELECT COUNT(*) AS players_num
                  FROM (players inner join sessions s on s.id = players.session_id)
                  WHERE game_id = game_id_
                  GROUP BY session_id) as players_nums);
end;
$$
    language plpgsql;


create or replace function get_participations(game_id_ varchar, user_id_ uuid)
    returns table
            (
                session_id uuid,
                player_id  integer,
                score      integer
            )
as
$$
declare
begin
    RETURN QUERY SELECT players.session_id, players.player_id, players.score
                 FROM (players inner join sessions s on s.id = players.session_id)
                 WHERE players.user_id = user_id_
                   AND s.game_id = game_id_;
end;
$$
    language plpgsql;

create or replace function get_total_time_sec_for_user(game_id_ varchar, user_id_ uuid) returns int as
$$
declare
begin
    RETURN (SELECT COALESCE(SUM(user_sessions.duration), 0)
            FROM (get_participations(game_id_, user_id_) p left join get_sessions_info(game_id_) s
                  on p.session_id = s.session_id) as user_sessions);
end;
$$
    language plpgsql;

create or replace function get_sessions_num_for_user(game_id_ varchar, user_id_ uuid) returns int as
$$
declare
begin
    RETURN (SELECT (COUNT(*) FILTER ( WHERE user_sessions.score is not null))
            FROM (get_participations(game_id_, user_id_) p left join get_sessions_info(game_id_) s
                  on p.session_id = s.session_id) as user_sessions);
end;
$$
    language plpgsql;

create or replace function get_win_rate(game_id_ varchar, user_id_ uuid) returns float as
$$
declare
begin
    RETURN (SELECT CAST((COUNT(*) FILTER ( WHERE p.score > 0)) AS FLOAT) /
                   NULLIF((COUNT(*) FILTER ( WHERE p.score is not null)), 0)
            FROM get_participations(game_id_, user_id_) as p);
end;
$$
    language plpgsql;

create or replace function all_games()
    returns table
            (
                game_id varchar
            )
as
$$
declare
begin
    RETURN QUERY (SELECT DISTINCT sessions.game_id FROM sessions);
end;
$$
    language plpgsql;


create or replace function get_cavoke_time_sec_for_user(user_id_ uuid) returns int as
$$
declare
begin
    RETURN (SELECT COALESCE(SUM(get_total_time_sec_for_user(g.game_id, user_id_)), 0)
            FROM all_games() as g);
end;
$$
    language plpgsql;


create or replace function get_cavoke_sessions_num_for_user(user_id_ uuid) returns int as
$$
declare
begin
    RETURN (SELECT SUM(get_sessions_num_for_user(g.game_id, user_id_))
            FROM all_games() as g);
end;
$$
    language plpgsql;
