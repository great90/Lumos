pillars = {}

local MAX_HEIGHT = 10.0
local SPEED = 7
local VerticalSpeed = 8
local m_FurthestPillarPosX = 0
local gapSize = 4.0;

local m_PillarTarget = 35.0
local m_PillarIndex = 1

local PLAYERTYPEID = 1
local PILLARTYPEID = 2

local GameStates = {
    Running=0,
    GameOver=1,
	Start
}

local gameState = GameStates.Start
local entityManager = {}

local player = {}
local camera = {}
local scoreEntity = {}
local score = 0
local iconTexture = nil
local gameOverTexture = nil
local gameOverEntity = nil
local gameOverScale = 1.0
local totalTime = 0.0
local gameOverSize = Vector2.new(30, 4)

function EndGame()
	gameState = GameStates.GameOver

    if gameOverEntity == nil then
    gameOverEntity = entityManager:Create()
    gameOverEntity:AddSprite(Vector2.new(0.0, 0.0), gameOverSize, Vector4.new(1.0, 1.0, 1.0, 1.0)):SetTexture(gameOverTexture)
	gameOverPos = player:GetTransform():GetWorldPosition() + Vector3.new(15.0, 2.0, 1.0)
	gameOverEntity:GetTransform():SetLocalScale(Vector3.new(0.2, 0.2, 0.2))
    end
end

function beginContact(a, b)
    EndGame()
end

function endContact(a, b, coll)
 
end
 
function preSolve(a, b, coll)
 
end
 
function postSolve(a, b, coll, normalimpulse, tangentimpulse)
 
end

function CreatePlayer()
    texture = LoadTextureWithParams("icon", "//Assets/Textures/TappyPlane/PNG/Planes/planeBlue1.png", TextureFilter.Linear, TextureWrap.ClampToEdge)

    player = entityManager:Create()
    player:AddSprite(Vector2.new(-0.9, -0.8), Vector2.new(1.7, 1.5), Vector4.new(1.0,1.0,1.0,1.0)):SetTexture(texture)

    local params = RigidBodyParameters.new()
    params.position = Vector3.new( 1.0, 1.0, 1.0)
    params.scale = Vector3.new(1.7 / 2.0, 1.5 / 2.0, 1.0)
	params.shape = Shape.Circle
	params.isStatic = false

    SetCallback(beginContact)

    player:GetTransform():SetLocalPosition(Vector3.new(1.0,1.0,1.0))
    player:AddRigidBody2DComponent(params):GetRigidBody():SetForce(Vector2.new(200.0,0.0))
    player:GetRigidBody2DComponent():GetRigidBody():SetIsStatic(true)
	player:GetRigidBody2DComponent():GetRigidBody():GetB2Body():SetLinearDamping(0.1)

end

function CreatePillar(index, offset)

	pillars[index] = entityManager:Create();
	pillars[index + 1] = entityManager:Create();

    local centre = Rand(-6.0, 6.0);
    local width = Rand(1.0, 2.0)
    
    --Top Pillar
	local topY = MAX_HEIGHT;
	local bottomY = centre + (gapSize / 2.0);

	local pos = Vector3.new(offset / 2.0, ((topY - bottomY )/ 2.0) + bottomY, 0.0);
	local scale = Vector3.new(width, (topY - bottomY) / 2.0, 1.0);

    pillars[index]:AddSprite(Vector2.new(-scale.x, -scale.y), Vector2.new(scale.x * 2.0, scale.y * 2.0), Vector4.new(1.0,1.0,1.0,1.0)):SetTexture(iconTexture)

    local params = RigidBodyParameters.new()
	params.position = pos
	params.scale = scale
	params.shape = Shape.Custom
	params.isStatic = true
	params.customShapePositions = { Vector2.new(-scale.x, -scale.y), Vector2.new(scale.x * 0.25, scale.y * 1.05), Vector2.new(scale.x, -scale.y)}

    pillars[index]:GetTransform():SetLocalPosition(pos)
    pillars[index]:AddRigidBody2DComponent(params):GetRigidBody():SetOrientation(3.14)

	--Bottom Pillar
	topY = centre - (gapSize / 2.0)
	bottomY = -MAX_HEIGHT;
    width = Rand(1.0, 2.0)
	pos = Vector3.new(offset / 2.0, ((topY - bottomY) / 2.0) + bottomY, 0.0)
	scale = Vector3.new(width, (topY - bottomY) / 2.0, 1.0)

    pillars[index + 1]:AddSprite(Vector2.new(-scale.x, -scale.y), Vector2.new(scale.x * 2.0, scale.y * 2.0), Vector4.new(1.0,1.0,1.0,1.0)):SetTexture(iconTexture)

	params.position = pos
	params.scale = scale
	params.shape = Shape.Custom
	params.isStatic = true
	params.customShapePositions = { Vector2.new(-scale.x, -scale.y), Vector2.new(scale.x * 0.25, scale.y * 1.05), Vector2.new(scale.x, -scale.y)}

    pillars[index + 1]:GetTransform():SetLocalPosition(pos)
    pillars[index + 1]:AddRigidBody2DComponent(params)

	if pos.x > m_FurthestPillarPosX then
        m_FurthestPillarPosX = pos.x;
    end
end

function CreateBackground(index)
    backgrounds[index] = entityManager:Create()
    backgrounds[index]:AddTransform():SetLocalPosition(Vector3.new(index * 20.0 - 40.0, 0.0, -1.0))
    backgrounds[index]:AddSprite(Vector2.new(-10.0, -10.0), Vector2.new(10.0 * 2.0, 10.0 * 2.0), Vector4.new(1.0,1.0,1.0,1.0)):SetTexture(backgroundTexture)
end

function updateSpeed(speed)
    SPEED = speed
end

function updateGap(gap)
    gapSize = gap
end

backgrounds = {}

function OnInit()
	iconTexture = LoadTextureWithParams("icon", "//Assets/Textures/TappyPlane/PNG/rock.png", TextureFilter.Linear, TextureWrap.ClampToEdge)
	gameOverTexture = LoadTextureWithParams("gameOver", "//Assets/Textures/TappyPlane/PNG/UI/textGameOver.png", TextureFilter.Linear, TextureWrap.ClampToEdge)

    entityManager = scene:GetEntityManager()

    camera = entityManager:Create()
	camera:AddTransform()

	scoreEntity = entityManager:Create()
	scoreEntity:AddTransform()
	scoreEntity:AddTextComponent()
	scoreEntity:GetTextComponent().Colour = Vector4.new(0.4, 0.1, 0.9, 1.0)
    scoreEntity:GetTextComponent().TextString = "Click to start!"
	scoreEntity:GetTransform():SetLocalPosition(Vector3.new(-4.0, 8.0, 0.0))
	scoreEntity:GetTransform():SetLocalScale(Vector3.new(2.0, 2.0, 2.0))

	local screenSize = GetAppInstance():GetWindowSize()
    camera:AddCamera(screenSize.x / screenSize.y, 10.0, 1.0)

	SetB2DGravity(Vector2.new(0.0, -18.0))
    CreatePlayer()

    for i=1,10, 2 do
        CreatePillar(i, (i + 2) * 10.0)
    end

    backgroundTexture = LoadTextureWithParams("background", "//Assets/Textures/TappyPlane/PNG/background.png", TextureFilter.Linear, TextureWrap.ClampToEdge)

    for i=1,50, 1 do
        CreateBackground(i)
    end
end

function PlayerJump()

	local phys = player:GetRigidBody2DComponent()
	local vel = phys:GetRigidBody():GetLinearVelocity()

	vel.y = VerticalSpeed
	vel.x = SPEED

    phys:GetRigidBody():SetLinearVelocity(vel)
end

function OnUpdate(dt)
	tracy.ZoneBegin()

	if Input.GetKeyPressed( Key.M ) then
    	SwitchScene()
    end
    if gameState == GameStates.Running then
		phys = player:GetRigidBody2DComponent()

        up = Vector3.new(0, 1, 0)
        right = Vector3.new(1, 0, 0)

        if Input.GetKeyHeld( Key.Space ) or Input.GetMouseClicked(MouseButton.Left) then
			PlayerJump()
        end

        pos = player:GetTransform():GetWorldPosition()

		if pos.y > MAX_HEIGHT or pos.y < -MAX_HEIGHT then
           	 EndGame()
        end

        pos.y = 0.0

        camera:GetTransform():SetLocalPosition(pos)
        
        score = math.max(math.floor((pos.x - 5) / 10), 0)

		scoreEntity:GetTransform():SetLocalPosition(pos + Vector3.new(0.0, 8.0, 0.0))
		scoreEntity:GetTextComponent().TextString = tostring(score)

        if pos.x > m_PillarTarget then
            
 			if pillars[m_PillarIndex] and pillars[m_PillarIndex]:Valid() then
				pillars[m_PillarIndex]:Destroy()
				pillars[m_PillarIndex+1]:Destroy()
			else
			    Log.Info("Not valid Pillar")
			    Log.Info(tostring(m_PillarIndex))
            end

 			CreatePillar(m_PillarIndex, m_FurthestPillarPosX * 2.0 + 20.0)
 			m_PillarIndex = m_PillarIndex + 2
		    m_PillarIndex = math.fmod(m_PillarIndex, 10)

			m_PillarTarget = m_PillarTarget + 10.0
        end 

    elseif gameState == GameStates.GameOver then

        totalTime = totalTime + dt * 2
        gameOverScale = 0.2 + (math.sin(totalTime) + 1.0) / 10.0
        gameOverEntity:GetTransform():SetLocalScale(Vector3.new(gameOverScale, gameOverScale, gameOverScale))
 
		gameOverEntity:GetTransform():SetLocalPosition(camera:GetTransform():GetWorldPosition() - Vector3.new((gameOverScale *  gameOverSize.x )/ 2, (gameOverScale *  gameOverSize.y )/ 2, -2.0))

		if Input.GetKeyPressed( Key.Space ) or Input.GetMouseClicked(MouseButton.Left) then
            Reset();
        end

	elseif gameState == GameStates.Start then

       scoreEntity:GetTextComponent().TextString = "Click to start"
	   scoreEntity:GetTransform():SetLocalPosition(Vector3.new(-4.0, 8.0, 0.0))

 	   pos = player:GetTransform():GetWorldPosition()
       pos.y = 0.0
 	   camera:GetTransform():SetLocalPosition(pos)
	 	if Input.GetKeyPressed( Key.Space ) or Input.GetMouseClicked(MouseButton.Left) then
			gameState = GameStates.Running
			player:GetRigidBody2DComponent():GetRigidBody():SetIsStatic(false)
			player:GetRigidBody2DComponent():GetRigidBody():SetForce(Vector2.new(200.0,0.0))
			PlayerJump()
     	end
	end

tracy.ZoneEnd()
end

function Reset()
    gameState = GameStates.Start
    phys = player:GetRigidBody2DComponent():GetRigidBody()

    phys:SetPosition(Vector2.new(0.0, 0.0))
    phys:SetForce(Vector2.new(400.0,0.0))

    phys:SetLinearVelocity(Vector2.new(0.0, 0.0))
	phys:SetOrientation(0.0)
	phys:SetAngularVelocity(0.0)
	phys:SetIsStatic(true)

	m_FurthestPillarPosX = 0.0
	m_PillarTarget = 35.0
    m_PillarIndex = 1

	player:GetTransform():SetLocalPosition(Vector3.new(0.0,0.0,0.0))

    for i=1,10, 2 do
        if pillars[i] and pillars[i]:Valid() then
            pillars[i]:Destroy();
            pillars[i + 1]:Destroy();

       	else
       	    Log.Info("Not valid Pillar")
       	    Log.Info(tostring(m_PillarIndex))
        end

        CreatePillar(i, (i + 2) * 10.0)
    end

    gameOverEntity:Destroy()
    gameOverEntity = nil

    collectgarbage()
end

function OnCleanUp()
    backgroundTexture = nil
    texture = nil
    blockPhysics = nil
    blockPhysics2 = nil
	iconTexture = nil
	gameOverTexture = nil
end

function OnRelease()
    OnCleanUp()
end



















































