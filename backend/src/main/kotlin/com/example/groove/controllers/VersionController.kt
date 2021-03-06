package com.example.groove.controllers

import org.springframework.http.ResponseEntity
import org.springframework.web.bind.annotation.*

@RestController
@RequestMapping("api/version")
class VersionController() {

	@GetMapping
	fun getVersion(): ResponseEntity<Map<String, String>> {
		return ResponseEntity.ok(mapOf("version" to "0.1.0"))
	}


}
