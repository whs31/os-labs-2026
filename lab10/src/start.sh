#!/bin/sh
php-fpm8.3 -D
exec nginx -g 'daemon off;'
