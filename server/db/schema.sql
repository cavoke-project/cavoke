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
    host_id       uuid null,
    game_settings json,
    status        integer,
    constraint fk_host foreign key (host_id) references users (id)
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
    session_id  uuid                  not null
        constraint globalstates_pk
            primary key
        constraint globalstates_sessions_id_fk
            references sessions
            on delete cascade,
    globalstate text,
    is_terminal boolean default false not null
);

create or replace function transfer_to_available(m_session_id uuid, m_user_id uuid) returns void as $$
declare
begin
    if (select host_id from sessions where id = m_session_id) = m_user_id then
        if (select user_id from players where session_id = m_session_id and user_id != m_user_id limit 1) IS NOT NULL then
            update sessions set host_id = (select user_id from players where session_id = m_session_id and user_id != m_user_id limit 1) where id = m_session_id;
        else
            delete from sessions where id = m_session_id;
        end if;
    end if;
    delete from players where user_id = m_user_id and session_id = m_session_id;
end $$ language plpgsql;
