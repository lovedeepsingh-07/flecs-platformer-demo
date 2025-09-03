STATES = {
	idle = {
		can_transition_to = MAKE_TRANSITION_TABLE(0, { "run", "jump", "attack", "hurt", "dash" }),
		animation_data = {
			loop = true,
			texture_id = "decidueye_idle",
			frames = {
				{ "active", { 0, 0, 57, 112 } },
				{ "active", { 57, 0, 57, 112 } },
				{ "active", { 114, 0, 57, 112 } },
				{ "active", { 171, 0, 57, 112 } },
				{ "active", { 228, 0, 57, 112 } },
				{ "active", { 285, 0, 57, 112 } },
				{ "active", { 342, 0, 57, 112 } },
				{ "active", { 399, 0, 57, 112 } },
			},
		},
	},
	hurt = {
		can_transition_to = MAKE_TRANSITION_TABLE(0, { "idle", "run", "jump" }),
		animation_data = {
			loop = false,
			texture_id = "decidueye_hurt",
			frames = {
				{ "windup", { 0, 0, 109, 110 } },
				{ "active", { 109, 0, 109, 110 } },
			},
		},
	},
	dash = {
		can_transition_to = MAKE_TRANSITION_TABLE(0, { "idle", "run", "attack", "attack_air", "hurt" }, { jump = -1 }),
		animation_data = {
			loop = false,
			texture_id = "decidueye_dash",
			frames = {
				{ "active", { 0, 0, 131, 122 } },
				{ "active", { 131, 0, 131, 122 } },
				{ "active", { 262, 0, 131, 122 } },
			},
		},
	},
	run = {
		can_transition_to = MAKE_TRANSITION_TABLE(0, { "idle", "jump", "attack", "hurt", "dash" }),
		animation_data = {
			loop = true,
			texture_id = "decidueye_run",
			frames = {
				{ "active", { 0, 0, 105, 113 } },
				{ "active", { 105, 0, 105, 113 } },
				{ "active", { 210, 0, 105, 113 } },
			},
		},
	},
	jump = {
		can_transition_to = MAKE_TRANSITION_TABLE(0, { "idle", "run", "attack_air", "hurt", "dash" }),
		animation_data = {
			loop = false,
			texture_id = "decidueye_jump",
			frames = {
				{ "active", { 0, 0, 141, 118 } },
				{ "active", { 141, 0, 141, 118 } },
				{ "active", { 282, 0, 141, 118 } },
				{ "active", { 423, 0, 141, 118 } },
				{ "active", { 564, 0, 141, 118 } },
			},
		},
	},
	attack = {
		hitbox = { 50, 0, 48, 32 },
		can_transition_to = MAKE_TRANSITION_TABLE(0, { "idle", "run", "jump", "hurt", "dash" }),
		animation_data = {
			loop = false,
			texture_id = "decidueye_attack",
			frames = {
				{ "windup", { 0, 0, 133, 143 } },
				{ "windup", { 133, 0, 133, 143 } },
				{ "active", { 266, 0, 133, 143 } },
				{ "recovery", { 399, 0, 133, 143 } },
			},
		},
	},
	attack_air = {
		hitbox = { 50, 0, 48, 32 },
		can_transition_to = MAKE_TRANSITION_TABLE(0, { "idle", "run", "hurt", "dash" }, { jump = -1 }),
		animation_data = {
			loop = false,
			texture_id = "decidueye_attack_air",
			frames = {
				{ "windup", { 0, 0, 163, 120 } },
				{ "active", { 163, 0, 163, 120 } },
				{ "recovery", { 326, 0, 163, 120 } },
			},
		},
	},
}
