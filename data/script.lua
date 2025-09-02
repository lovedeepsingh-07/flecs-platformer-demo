function MAKE_TRANSITION_TABLE(default_value, input_table, override_table)
	local output_table = {}
	for _, key in ipairs(input_table) do
		output_table[key] = default_value
	end
	if override_table then
		for k, v in pairs(override_table) do
			output_table[k] = v
		end
	end
	return output_table
end
