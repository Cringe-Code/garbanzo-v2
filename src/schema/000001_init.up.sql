create table if not exists users (
    id serial primary key,
    login text,
    phone text,
    email text,
    hash_password text
);

create table if not exists tokens (
    user_id bigint,
    access_token text,
    refresh_token text
);

