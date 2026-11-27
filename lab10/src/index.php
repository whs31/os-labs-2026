<?php
echo "Hello from nginx + PHP-FPM (whs31)", PHP_EOL;
echo "PHP ", PHP_VERSION, ", SAPI: ", php_sapi_name(), PHP_EOL;
echo "Host: ", gethostname(), PHP_EOL;
