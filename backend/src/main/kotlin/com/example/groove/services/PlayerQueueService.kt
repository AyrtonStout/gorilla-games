package com.example.groove.services

import com.example.groove.db.dao.GameRepository
import com.example.groove.db.dao.PlayerQueueRepository
import com.example.groove.db.model.Game
import com.example.groove.util.unwrap
import org.slf4j.LoggerFactory
import org.springframework.stereotype.Service
import org.springframework.transaction.annotation.Propagation
import org.springframework.transaction.annotation.Transactional
import java.sql.Timestamp


@Service
class PlayerQueueService(
		private val playerQueueRepository: PlayerQueueRepository,
		private val gameRepository: GameRepository
) {

	@Transactional
	fun findMatch(queueId: Long): Long? {
		synchronized(this) {
			val dbQueue = playerQueueRepository.findById(queueId).unwrap()
					?: throw IllegalArgumentException("No waiting queue found for ID $queueId")

			dbQueue.updated = Timestamp(System.currentTimeMillis())
			playerQueueRepository.save(dbQueue)

			// The other person in the queue "found" us, and already assigned us a game to play. We want to return this
			if (dbQueue.game != null) {
				val foundGame = dbQueue.game!!.id
				playerQueueRepository.delete(dbQueue)

				logger.info("Already found a game for queue ID: $queueId. Deleting queue and moving on")
				return foundGame
			}

			// Find any queue that has been updated in the last 20 seconds as anything older probably isn't alive
			val newerThan = Timestamp(System.currentTimeMillis() - 20_000)
			val validMatch = playerQueueRepository.findPlayerQueueForMatchMaking(dbQueue.id, newerThan)
					?: return null

			logger.info("A valid match was found! Queue ID: $queueId matched with queue ID: ${validMatch.id}")
			// We have found a valid person to match with. Create a game for them to play
			val newGame = Game()
			gameRepository.saveAndFlush(newGame)

			logger.info("Queue ID: ${validMatch.id} should now be using game ID ${newGame.id}")
			validMatch.game = newGame
			playerQueueRepository.save(validMatch)

			// Delete our own queue. Keep the other one so the other player is notified of the game to play
			logger.info("Deleting queue ID: ${dbQueue.id}")
			playerQueueRepository.delete(dbQueue)

			return newGame.id
		}
	}

	companion object {
		val logger = LoggerFactory.getLogger(PlayerQueueService::class.java)!!
	}
}
