package com.example.groove.controllers

import com.example.groove.db.dao.GameMoveRepository
import com.example.groove.db.dao.GameRepository
import com.example.groove.db.model.GameMove
import org.springframework.http.HttpStatus
import org.springframework.http.ResponseEntity
import org.springframework.transaction.annotation.Transactional

import org.springframework.web.bind.annotation.*

@RestController
@RequestMapping("api/game-move")
class GameMoveController(
		private val gameMoveRepository: GameMoveRepository,
		private val gameRepository: GameRepository
) {

	@Transactional
	@PostMapping("/game/{gameId}/move")
	fun createMove(
			@PathVariable gameId: Long,
			@RequestBody createGameMoveDTO: CreateGameMoveDTO
	): ResponseEntity<String> {
		// If I wasn't too lazy to figure out how to wire in an EntityManager to load a proxy object I'd do that and avoid the DB hit
		val game = gameRepository.getOne(gameId)
		GameMove(
				game = game,
				actionPerformed = createGameMoveDTO.actionPerformed,
				playerGuid = createGameMoveDTO.playerGuid
		).also { gameMoveRepository.save(it) }

		return ResponseEntity
				.status(HttpStatus.CREATED)
				.build()
	}

	@Transactional
	@GetMapping("/game/{gameId}/moves")
	fun waitForQueue(
			@PathVariable gameId: Long,
			@RequestParam since: Long
	): List<GameMove> {
		// Long poll until a game is found
		for (i in 1..TIMES_TO_CHECK) {
			val newMoves = gameMoveRepository.findGameMovesNewerThan(
					gameId = gameId,
					lastSeenId = since
			)
			if (newMoves.isNotEmpty()) {
				return newMoves
			}
			Thread.sleep(SECONDS_BETWEEN_CHECKS * 1000L)
		}

		return emptyList()
	}

	companion object {
		const val SECONDS_BETWEEN_CHECKS = 1
		const val TIMES_TO_CHECK = 10
	}


	data class CreateGameMoveDTO(
			val actionPerformed: Int,
			val playerGuid: String
	)
}
