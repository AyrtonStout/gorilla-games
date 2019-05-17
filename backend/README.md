# Gorilla Games

## Installation

Copy `/config/application.properties.cfg` to `/config/application.properties`. Environment specific setup will go in here, and likely every item in it will need to be configured for the server to run successfully.


### Database
Gorilla Games uses MySQL. Likely any modern MySQL will work.

Edit `/config/application.properties` and set `spring.datasource.username` and `spring.datasource.password` to be a MySQL user
Then edit `spring.datasource.url` to be accurate for your database's IP and port


## Running

If using Java 10, add `--add-modules java.xml.bind` to the JVM arguments or you will see a ClassNotFoundException for JAXB

When the projects starts up, it will use Flyway to migrate the database to the latest version.

To verify that it is working, try hitting `http://localhost:8080/api/version`
