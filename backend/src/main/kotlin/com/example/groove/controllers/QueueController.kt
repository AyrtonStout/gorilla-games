package com.example.groove.controllers

import com.example.groove.db.dao.PlayerQueueRepository
import com.example.groove.db.model.PlayerQueue
import com.example.groove.services.PlayerQueueServiceImpl
import org.slf4j.LoggerFactory

import org.springframework.web.bind.annotation.*

@RestController
@RequestMapping("api/queue")
class QueueController(
		private val playerQueueRepository: PlayerQueueRepository,
		private val playerQueueService: PlayerQueueServiceImpl
) {

	@PostMapping
	fun enterQueue(): QueueIdDTO {
		val newQueue = PlayerQueue().also { playerQueueRepository.save(it) }

		return QueueIdDTO(id = newQueue.id)
	}

	@GetMapping("/{queueId}")
	fun waitForQueue(@PathVariable queueId: Long): QueueWaitResponse {
		// Long poll until a game is found
		// FIXME this doesn't actually work. The new game is never found until this method is recalled
		for (i in 1..TIMES_TO_CHECK) {
            val gameId = synchronized(this) {
                return@synchronized playerQueueService.findMatch(queueId = queueId)
			}
			if (gameId != null) {
				return QueueWaitResponse(gameId = gameId)
			}
			Thread.sleep(SECONDS_BETWEEN_CHECKS * 1000L)
		}

		return QueueWaitResponse(gameId = null)
	}

	companion object {
		const val SECONDS_BETWEEN_CHECKS = 1
		const val TIMES_TO_CHECK = 1

		val logger = LoggerFactory.getLogger(QueueController::class.java)!!
	}


	data class QueueIdDTO(
			val id: Long
	)

	data class QueueWaitResponse(
			val gameId: Long?
	)
}
