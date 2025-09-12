STATES = {
	idle = {
		can_transition_to = MAKE_TRANSITION_TABLE(
			0,
			{ "run", "jump", "light_forward", "light_down", "heavy_forward", "heavy_down", "hurt", "dash", "block" }
		),
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
		can_transition_to = MAKE_TRANSITION_TABLE(0, {
			"idle",
			"run",
			"light_forward",
			"light_down",
			"heavy_forward",
			"heavy_down",
			"light_forward_air",
			"light_down_air",
			"heavy_forward_air",
			"hurt",
			"block",
		}, { jump = -1 }),
		animation_data = {
			loop = false,
			texture_id = "cowboy_dash",
			frames = {
				{ "active", { 0, 0, 128, 128 } },
			},
		},
	},
	dash_air = {
		can_transition_to = MAKE_TRANSITION_TABLE(0, {
			"idle",
			"run",
			"light_forward",
			"light_down",
			"heavy_forward",
			"heavy_down",
			"light_forward_air",
			"light_down_air",
			"heavy_forward_air",
			"hurt",
			"block_air",
		}, { jump = -1 }),
		animation_data = {
			loop = false,
			texture_id = "cowboy_dash_air",
			frames = {
				{ "active", { 0, 0, 128, 128 } },
			},
		},
	},
	run = {
		can_transition_to = MAKE_TRANSITION_TABLE(0, {
			"idle",
			"jump",
			"light_forward",
			"light_down",
			"heavy_forward",
			"heavy_down",
			"hurt",
			"dash",
			"block",
		}),
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
		can_transition_to = MAKE_TRANSITION_TABLE(0, {
			"idle",
			"run",
			"light_forward_air",
			"light_down_air",
			"heavy_forward_air",
			"hurt",
			"dash_air",
			"block_air",
		}),
		animation_data = {
			loop = false,
			texture_id = "cowboy_jump",
			frames = {
				{ "active", { 0, 0, 128, 128 } },
			},
		},
	},
	light_forward = {
		damage = 10,
		hitbox = { 50, 0, 48, 32 },
		can_transition_to = MAKE_TRANSITION_TABLE(0, { "idle", "run", "jump", "hurt", "dash", "block" }),
		animation_data = {
			loop = false,
			texture_id = "cowboy_sword_slash_horizontal",
			frames = {
				{ "windup", { 0, 0, 512, 256 } },
				{ "active", { 512, 0, 512, 256 } },
				{ "recover", { 1024, 0, 512, 256 } },
				{ "recover", { 1536, 0, 512, 256 } },
			},
		},
	},
	light_down = {
		damage = 10,
		hitbox = { 50, 0, 48, 32 },
		can_transition_to = MAKE_TRANSITION_TABLE(0, { "idle", "run", "jump", "hurt", "dash", "block" }),
		animation_data = {
			loop = false,
			texture_id = "cowboy_sword_slash_horizontal_up",
			frames = {
				{ "windup", { 0, 0, 512, 256 } },
				{ "active", { 512, 0, 512, 256 } },
				{ "recover", { 1024, 0, 512, 256 } },
				{ "recover", { 1536, 0, 512, 256 } },
			},
		},
	},
	heavy_forward = {
		damage = 20,
		hitbox = { 50, 0, 48, 32 },
		can_transition_to = MAKE_TRANSITION_TABLE(0, { "idle", "run", "jump", "hurt", "dash", "block" }),
		animation_data = {
			loop = false,
			texture_id = "cowboy_sword_slash_horizontal_big",
			frames = {
				{ "windup", { 0, 0, 512, 256 } },
				{ "active", { 512, 0, 512, 256 } },
				{ "recover", { 1024, 0, 512, 256 } },
			},
		},
	},
	heavy_down = {
		damage = 20,
		hitbox = { 50, 0, 48, 32 },
		can_transition_to = MAKE_TRANSITION_TABLE(0, { "idle", "run", "jump", "hurt", "dash", "block" }),
		animation_data = {
			loop = false,
			texture_id = "cowboy_sword_slash_vertical",
			frames = {
				{ "windup", { 0, 0, 256, 256 } },
				{ "active", { 256, 0, 256, 256 } },
				{ "recover", { 512, 0, 256, 256 } },
				{ "recover", { 768, 0, 256, 256 } },
			},
		},
	},
	light_forward_air = {
		damage = 10,
		hitbox = { 50, 0, 48, 32 },
		can_transition_to = MAKE_TRANSITION_TABLE(0, { "idle", "run", "hurt", "dash_air", "block_air" }, { jump = -1 }),
		animation_data = {
			loop = false,
			texture_id = "cowboy_horizontal_air_slash",
			frames = {
				{ "windup", { 0, 0, 128, 128 } },
				{ "active", { 128, 0, 128, 128 } },
				{ "recovery", { 256, 0, 128, 128 } },
			},
		},
	},
	light_down_air = {
		damage = 10,
		hitbox = { 50, 0, 48, 32 },
		can_transition_to = MAKE_TRANSITION_TABLE(0, { "idle", "run", "hurt", "dash_air", "block_air" }, { jump = -1 }),
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
	heavy_forward_air = {
		damage = 20,
		hitbox = { 50, 0, 48, 32 },
		can_transition_to = MAKE_TRANSITION_TABLE(0, { "idle", "run", "hurt", "dash_air", "block_air" }, { jump = -1 }),
		animation_data = {
			loop = false,
			texture_id = "cowboy_triple_ass_smacker_1",
			frames = {
				{ "windup", { 0, 0, 128, 128 } },
				{ "windup", { 128, 0, 128, 128 } },
				{ "active", { 256, 0, 128, 128 } },
				{ "recovery", { 384, 0, 128, 128 } },
			},
		},
	},
	block = {
		can_transition_to = MAKE_TRANSITION_TABLE(0, {
			"idle",
			"run",
			"light_forward",
			"light_down",
			"heavy_forward",
			"heavy_down",
			"jump",
			"hurt",
			"dash",
			"block_air",
		}),
		animation_data = {
			loop = false,
			texture_id = "cowboy_shield_high",
			frames = {
				{ "active", { 0, 0, 128, 128 } },
			},
		},
	},
	block_air = {
		can_transition_to = MAKE_TRANSITION_TABLE(0, {
			"idle",
			"run",
			"light_forward_air",
			"light_down_air",
			"heavy_forward_air",
			"hurt",
			"dash_air",
			"block",
		}, { jump = -1 }),
		animation_data = {
			loop = false,
			texture_id = "cowboy_shield_low",
			frames = {
				{ "active", { 0, 0, 128, 128 } },
			},
		},
	},
}
