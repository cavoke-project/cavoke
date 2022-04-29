create table users
(
    id varchar not null
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
    game_settings json,
    status        integer
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
