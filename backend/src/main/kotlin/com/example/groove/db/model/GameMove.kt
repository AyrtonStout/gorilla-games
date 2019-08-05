package com.example.groove.db.model

import com.fasterxml.jackson.annotation.JsonIgnore
import java.sql.Timestamp
import javax.persistence.*

@Entity
@Table(name = "game_move")
data class GameMove(

		@Id
		@GeneratedValue(strategy = GenerationType.IDENTITY)
		val id: Long = 0,

        @JsonIgnore
		@ManyToOne(fetch = FetchType.LAZY)
		@JoinColumn(name = "game_id")
		val game: Game,

        @Column(name = "action_performed")
		val actionPerformed: Int,

        @Column
		val created: Timestamp = Timestamp(System.currentTimeMillis())
)

