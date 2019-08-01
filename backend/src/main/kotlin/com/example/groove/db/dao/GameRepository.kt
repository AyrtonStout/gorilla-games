package com.example.groove.db.dao

import com.example.groove.db.model.Game
import org.springframework.data.jpa.repository.JpaRepository

interface GameRepository : JpaRepository<Game, Long>
