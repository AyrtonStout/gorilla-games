CREATE TABLE `gorilla_games`.`game` (
  `id` INT UNSIGNED NOT NULL AUTO_INCREMENT,
  `created` DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`),
  UNIQUE INDEX `id_UNIQUE` (`id` ASC));

CREATE TABLE `gorilla_games`.`player_queue` (
  `queue_id` INT UNSIGNED NOT NULL AUTO_INCREMENT,
  `updated` DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`queue_id`),
  UNIQUE INDEX `queue_id_UNIQUE` (`queue_id` ASC));

ALTER TABLE `gorilla_games`.`player_queue`
  ADD COLUMN `created` DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP AFTER `updated`;

ALTER TABLE `gorilla_games`.`player_queue`
  CHANGE COLUMN `queue_id` `id` INT(10) UNSIGNED NOT NULL AUTO_INCREMENT;

ALTER TABLE `gorilla_games`.`player_queue`
  ADD COLUMN `game_id` INT NULL AFTER `created`;

ALTER TABLE `gorilla_games`.`player_queue`
  CHANGE COLUMN `game_id` `game_id` INT(11) UNSIGNED NULL DEFAULT NULL ;

ALTER TABLE `gorilla_games`.player_queue
  ADD CONSTRAINT fk_player_queue_game
  FOREIGN KEY (game_id) REFERENCES `gorilla_games`.game(id);
