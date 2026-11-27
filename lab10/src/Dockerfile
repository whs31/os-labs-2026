FROM ubuntu:24.04
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y --no-install-recommends nginx php8.3-fpm \
 && rm -rf /var/lib/apt/lists/*
COPY default.conf /etc/nginx/sites-available/default
COPY index.php /var/www/html/index.php
COPY start.sh /start.sh
RUN chmod +x /start.sh
EXPOSE 80
CMD ["/start.sh"]
