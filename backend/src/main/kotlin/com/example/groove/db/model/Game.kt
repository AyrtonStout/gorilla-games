package com.example.groove.db.model

import java.sql.Timestamp
import javax.persistence.*

@Entity
data class Game(

		@Id
		@GeneratedValue(strategy = GenerationType.IDENTITY)
		val id: Long = 0,

		var created: Timestamp = Timestamp(System.currentTimeMillis())
)

