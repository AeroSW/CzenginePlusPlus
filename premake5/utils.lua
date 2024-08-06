function prettyPrintTable(o)
    if type(o) == 'table' then
        local s = '{ '
        for k,v in pairs(o) do
            if type(k) ~= 'number' then k = '"'..k..'"' end
            s = s .. '['..k..'] = ' .. prettyPrintTable(v) .. ','
        end
        return s .. '} '
    else
        return tostring(o)
    end
end

function exists(file)
    local ok, err, code = os.rename(file, file)
    if not ok then
        if code == 13 then
            -- Permission denied, but it exists
            return true
        end
    end
    return ok, err
end
 
function isdir(path)
    -- "/" works on both Unix and Windows
    return exists(path.."/")
end