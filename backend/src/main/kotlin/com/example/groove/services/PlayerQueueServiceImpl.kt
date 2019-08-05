package com.example.groove.services

import com.example.groove.db.dao.GameRepository
import com.example.groove.db.dao.PlayerQueueRepository
import com.example.groove.db.model.Game
import com.example.groove.util.unwrap
import org.slf4j.LoggerFactory
import org.springframework.stereotype.Service
import org.springframework.transaction.annotation.Transactional
import java.sql.Timestamp
import org.springframework.transaction.annotation.Isolation


@Service
@Transactional(isolation = Isolation.READ_UNCOMMITTED)
class PlayerQueueServiceImpl(
        private val playerQueueRepository: PlayerQueueRepository,
        private val gameRepository: GameRepository
) : PlayerQueueService {

//    @Autowired
//    private val transactionTemplate: TransactionTemplate? = null

    // FIXME No matter what I try, this thing does not get the most up to date information when a PRE-EXISTING entity has its updated date changed
    // Reproduce:
    // 1) Create two queues. Wait 5 seconds so that they are both stale
    // 2) Pick 1 queue and poll for matches
    // 3) Pick the other queue and poll for matches. It will immediately find a match, but the other queue WILL NOT despite the database being updated
    override fun findMatch(queueId: Long): Long? {
//        transactionTemplate!!.propagationBehavior = Propagation.REQUIRES_NEW.value()
//        transactionTemplate.isolationLevel = Isolation.READ_COMMITTED.value()
//
//        return transactionTemplate.execute {
            return internalStuff(queueId)
//        }
    }

    fun internalStuff(queueId: Long): Long? {
        val ourQueue = playerQueueRepository.findById(queueId).unwrap()
                ?: throw IllegalArgumentException("No waiting queue found for ID $queueId")

        playerQueueRepository.updateUpdatedDateForPlayerQueue(ourQueue.id, Timestamp(System.currentTimeMillis()))

        // The other person in the queue "found" us, and already assigned us a game to play. We want to return this
        if (ourQueue.game != null) {
            return ourQueue.game!!.id
        }

        // Find any queue that has been updated in the last 5 seconds as anything older probably isn't alive
        val newerThan = Timestamp(System.currentTimeMillis() - 5_000)
        val matchedQueue = playerQueueRepository.findPlayerQueueForMatchMaking(ourQueue.id, newerThan)
                ?: return null

        logger.info("A valid match was found! Queue ID: $queueId matched with queue ID: ${matchedQueue.id}")
        // We have found a valid person to match with. Create a game for them to play
        val newGame = Game()
        gameRepository.saveAndFlush(newGame)

        logger.info("Queue ID: ${matchedQueue.id} should now be using game ID ${newGame.id}")
        matchedQueue.game = newGame
        matchedQueue.matched = true
        playerQueueRepository.save(matchedQueue)

        ourQueue.matched = true
        ourQueue.game = newGame
        playerQueueRepository.save(ourQueue)

        return newGame.id
    }

    companion object {
        val logger = LoggerFactory.getLogger(PlayerQueueServiceImpl::class.java)!!
    }
}
