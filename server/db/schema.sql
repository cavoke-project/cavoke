create table users
(
    id uuid not null
        constraint user_pk
            primary key
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
    user_id     uuid    not null
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

create or replace function get_average_session_time_sec(game_id_ varchar) returns int as
$$
declare
begin
    RETURN (SELECT EXTRACT(epoch from AVG(durations.duration))
            FROM (SELECT MAX(saved_on) - MIN(saved_on) as duration
                  FROM (statuses inner join sessions s on s.id = statuses.session_id)
                  WHERE game_id = game_id_
                    and (status = 1 or status = 2)
                  GROUP BY session_id) as durations);
end
$$
    language plpgsql;
