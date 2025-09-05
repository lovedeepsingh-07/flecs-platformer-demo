STATES = {
	idle = {
		can_transition_to = MAKE_TRANSITION_TABLE(0, { "run", "jump", "attack", "hurt", "dash" }),
		animation_data = {
			loop = true,
			texture_id = "cowboy_idle",
			frames = {
				{ "active", { 0, 0, 128, 128 } },
			},
		},
	},
	hurt = {
		can_transition_to = MAKE_TRANSITION_TABLE(0, { "idle", "run", "jump" }),
		animation_data = {
			loop = false,
			texture_id = "cowboy_hurt",
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
			texture_id = "cowboy_air_dash",
			frames = {
				{ "active", { 0, 0, 128, 128 } },
			},
		},
	},
	run = {
		can_transition_to = MAKE_TRANSITION_TABLE(0, { "idle", "jump", "attack", "hurt", "dash" }),
		animation_data = {
			loop = true,
			texture_id = "cowboy_run",
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
			texture_id = "cowboy_jump",
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
			texture_id = "cowboy_sword_slash_horizontal_up",
			frames = {
				{ "windup", { 0, 0, 512, 256 } },
				{ "active", { 512, 0, 512, 256 } },
				{ "recovery", { 1024, 0, 512, 256 } },
				{ "recovery", { 1536, 0, 512, 256 } },
			},
		},
	},

	attack_air = {
		hitbox = { 50, 0, 48, 32 },
		can_transition_to = MAKE_TRANSITION_TABLE(0, { "idle", "run", "hurt", "dash" }, { jump = -1 }),
		animation_data = {
			loop = false,
			texture_id = "cowboy_vertical_air_slash",
			frames = {
				{ "windup", { 0, 0, 128, 128 } },
				{ "active", { 128, 0, 128, 128 } },
				{ "recovery", { 256, 0, 128, 128 } },
			},
		},
	},
}
