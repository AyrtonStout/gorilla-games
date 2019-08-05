package com.example.groove.services

interface PlayerQueueService {
    fun findMatch(queueId: Long): Long?
}