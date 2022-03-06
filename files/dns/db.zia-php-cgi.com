;
$TTL    604800
@       IN      SOA     zia-php-cgi.com. root.zia-php-cgi.com. (
                             16         ; Serial
                         604800         ; Refresh
                          86400         ; Retry
                        2419200         ; Expire
                         604800 )       ; Negative Cache TTL

;
@               IN      NS      zia-php-cgi.com.

;
@               IN      A       192.168.43.187

;
@               IN      MX      10              mail.zia-php-cgi.com.

;
www             IN      A       192.168.43.187
mail            IN      A       192.168.43.187
www.mail        IN      A       192.168.43.187
mailadmin       IN      A       192.168.43.187
www.mailadmin   IN      A       192.168.43.187
