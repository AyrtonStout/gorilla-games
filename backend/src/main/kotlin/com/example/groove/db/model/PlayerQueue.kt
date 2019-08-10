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
		var game: Game? = null,

		@Column(columnDefinition = "BIT") // MySQL lacks a Boolean type. Need to label it as a BIT column
		var matched: Boolean = false,

		@Column
		val playerGuid: String
)

