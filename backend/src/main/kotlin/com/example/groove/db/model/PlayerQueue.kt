package com.example.groove.db.model

import java.sql.Timestamp
import javax.persistence.*

@Entity
@Table(name = "player_queue")
data class PlayerQueue(

		@Id
		@GeneratedValue(strategy = GenerationType.IDENTITY)
		val id: Long = 0,

		var created: Timestamp = Timestamp(System.currentTimeMillis()),

		var updated: Timestamp = Timestamp(System.currentTimeMillis()),

		@ManyToOne(fetch = FetchType.LAZY)
		@JoinColumn(name = "game_id")
		var game: Game? = null
)

