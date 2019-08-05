ALTER TABLE `gorilla_games`.`player_queue`
  ADD COLUMN `matched` BIT NOT NULL AFTER `game_id`;

