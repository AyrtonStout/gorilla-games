CREATE TABLE `gorilla_games`.`game_move` (
  `id` BIGINT UNSIGNED NOT NULL AUTO_INCREMENT,
  `game_id` INT UNSIGNED NOT NULL,
  `action_performed` INT NOT NULL,
  PRIMARY KEY (`id`),
  UNIQUE INDEX `id_UNIQUE` (`id` ASC) VISIBLE);

ALTER TABLE `gorilla_games`.game_move
  ADD CONSTRAINT fk_player_game_move_game
  FOREIGN KEY (game_id) REFERENCES `gorilla_games`.game(id);

ALTER TABLE `gorilla_games`.`game_move`
  ADD COLUMN `created` DATETIME NOT NULL DEFAULT CURRENT_TIMESTAMP AFTER `action_performed`;
