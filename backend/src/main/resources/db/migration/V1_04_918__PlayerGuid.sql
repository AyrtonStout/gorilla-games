ALTER TABLE `gorilla_games`.`player_queue`
ADD COLUMN `player_guid` CHAR(36) NOT NULL AFTER `matched`;

ALTER TABLE `gorilla_games`.`game_move`
ADD COLUMN `player_guid` CHAR(36) NOT NULL AFTER `created`;
