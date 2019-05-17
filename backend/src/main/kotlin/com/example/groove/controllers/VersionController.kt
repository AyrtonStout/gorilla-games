package com.example.groove.controllers

import com.example.groove.db.dao.UserRepository
import org.springframework.http.ResponseEntity
import org.springframework.web.bind.annotation.*

@RestController
@RequestMapping("api/version")
class VersionController(
		private val userRepository: UserRepository
) {

	@GetMapping
	fun getVersion(): ResponseEntity<Map<String, String>> {
		userRepository.findByEmail("hi");

		return ResponseEntity.ok(mapOf("version" to "0.1.0"))
	}


}
