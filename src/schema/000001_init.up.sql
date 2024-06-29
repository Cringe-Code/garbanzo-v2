create table if not exists users (
    id serial primary key,
    login text,
    phone text,
    email text,
    hash_password text,
    is_admin boolean
);

create table if not exists tokens (
    user_id bigint,
    refresh_token text,
    device_id bigint
);

create table if not exists item (
    id text primary key,
    title text,
    preview_link text,
    description text,
    weight bigint,
    cost bigint,
    rating bigint
);

create table if not exists in_cart (
    user_id bigint,
    item_id text,
    count bigint,
    foreign key (user_id) references users(id),
    foreign key (item_id) references item(id)
);
