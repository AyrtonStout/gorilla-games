-- MySQL is apparently like the only DB that has a default isolation level of REPEATABLE READ
-- This screws up long polling because our transaction will never get new data
SET GLOBAL TRANSACTION ISOLATION LEVEL READ COMMITTED;
