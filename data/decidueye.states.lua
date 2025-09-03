STATES = {
	idle = {
		can_transition_to = MAKE_TRANSITION_TABLE(0, { "run", "jump", "attack", "hurt", "dash" }),
		animation_data = {
			loop = true,
			texture_id = "decidueye_idle",
			frames = {
				{ "active", { 0, 0, 128, 128 } },
			},
		},
	},
	hurt = {
		can_transition_to = MAKE_TRANSITION_TABLE(0, { "idle", "run", "jump" }),
		animation_data = {
			loop = false,
			texture_id = "decidueye_hurt",
			frames = {
				{ "windup", { 0, 0, 128, 128 } },
				{ "windup", { 128, 0, 128, 128 } },
				{ "active", { 256, 0, 128, 128 } },
				{ "recovery", { 384, 0, 128, 128 } },
			},
		},
	},
	dash = {
		can_transition_to = MAKE_TRANSITION_TABLE(0, { "idle", "run", "attack", "attack_air", "hurt" }, { jump = -1 }),
		animation_data = {
			loop = false,
			texture_id = "decidueye_dash",
			frames = {
				{ "active", { 0, 0, 128, 128 } },
			},
		},
	},
	run = {
		can_transition_to = MAKE_TRANSITION_TABLE(0, { "idle", "jump", "attack", "hurt", "dash" }),
		animation_data = {
			loop = true,
			texture_id = "decidueye_run",
			frames = {
				{ "active", { 0, 0, 128, 128 } },
				{ "active", { 128, 0, 128, 128 } },
				{ "active", { 256, 0, 128, 128 } },
			},
		},
	},

	jump = {
		can_transition_to = MAKE_TRANSITION_TABLE(0, { "idle", "run", "attack_air", "hurt", "dash" }),
		animation_data = {
			loop = false,
			texture_id = "decidueye_jump",
			frames = {
				{ "active", { 0, 0, 128, 128 } },
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
				{ "windup", { 0, 0, 256, 256 } },
				{ "active", { 256, 0, 256, 256 } },
				{ "recovery", { 512, 0, 256, 256 } },
				{ "recovery", { 768, 0, 256, 256 } },
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
				{ "windup", { 0, 0, 128, 128 } },
				{ "active", { 128, 0, 128, 128 } },
				{ "recovery", { 256, 0, 128, 128 } },
			},
		},
	},
}
