CREATE DATABASE IF NOT EXISTS `gorilla_games`;

CREATE TABLE `gorilla_games`.`user` (
  `id` INT(11) UNSIGNED NOT NULL AUTO_INCREMENT,
  `name` VARCHAR(64) NOT NULL,
  `email` VARCHAR(255) NOT NULL,
  `password` VARCHAR(255),
  PRIMARY KEY (`id`));

CREATE TABLE `gorilla_games`.`user_token` (
  `id` int(10) UNSIGNED NOT NULL AUTO_INCREMENT,
  `user_id` int(10) UNSIGNED NOT NULL,
  `token` char(36) NOT NULL,
  `created_at` TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  KEY `FKraru0qloxyh7wacxneiam2hi7` (`user_id`),
  CONSTRAINT `FKraru0qloxyh7wacxneiam2hi7` FOREIGN KEY (`user_id`) REFERENCES `user` (`id`) ON DELETE CASCADE ON UPDATE CASCADE
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=latin1;
