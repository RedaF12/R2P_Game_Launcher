local genPlay = {}
function genPlay:OnClick()
    
    local path = [[C:\Games\C&C Generals - Zero hour\Command and Conquer Generals\generals.exe]]
    
    
    local result = os.execute('start "" "' .. path .. '"')
    
    if result then
    else
    end
end

genPlay:OnClick()
