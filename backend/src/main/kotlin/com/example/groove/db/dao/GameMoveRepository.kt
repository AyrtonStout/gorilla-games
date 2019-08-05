package com.example.groove.db.dao

import com.example.groove.db.model.GameMove
import org.springframework.data.jpa.repository.JpaRepository
import org.springframework.data.jpa.repository.Query

interface GameMoveRepository : JpaRepository<GameMove, Long> {
	@Query("""
		SELECT gm
		FROM GameMove gm
		WHERE game_id = :gameId
		AND gm.id > :lastSeenId
		ORDER BY gm.id ASC
	""")
	fun findGameMovesNewerThan(gameId: Long, lastSeenId: Long): List<GameMove>
}
