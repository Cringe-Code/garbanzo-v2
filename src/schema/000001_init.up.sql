create table if not exists users (
    id serial primary key,
    login text,
    email text,
    hash_password text
);

