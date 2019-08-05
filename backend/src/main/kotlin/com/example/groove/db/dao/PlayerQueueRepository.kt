package com.example.groove.db.dao

import com.example.groove.db.model.PlayerQueue
import org.springframework.data.jpa.repository.JpaRepository
import org.springframework.data.jpa.repository.Modifying
import org.springframework.data.jpa.repository.Query
import java.sql.Timestamp

interface PlayerQueueRepository : JpaRepository<PlayerQueue, Long> {
	@Query("""
		SELECT pq
		FROM PlayerQueue pq
		WHERE id <> :id
        AND pq.matched = false
		AND pq.updated > :newerThan
		ORDER BY pq.created ASC
	""")
	fun findPlayerQueueForMatchMaking(id: Long, newerThan: Timestamp): PlayerQueue?

	@Modifying
	@Query("""
    UPDATE PlayerQueue pq
    SET pq.updated = :updated
    WHERE pq.id = :id
    """)
	fun updateUpdatedDateForPlayerQueue(id: Long, updated: Timestamp)
}
