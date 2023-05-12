extends KinematicBody2D

var motion = Vector2(0, 0)

const SPEED = 1000
const GRAVITY = 300
const UP = Vector2(0, -1) # in a platformer, up means negative y axis
const JUMP_SPEED = 6000
const WORLD_LIMIT = 5000
var lives = 3
var switch_character = false

func _physics_process(delta):
	apply_gravity()
	jump()
	move()
	animate()
	switch_character()

	
	# tell move and slide to move according to player
	# also tell it what "UP" is so that gravity works properly
	move_and_slide(motion, UP)
	
	
func switch_character():
		# listen for switch character
	if Input.is_action_just_pressed("switch_character"):
		switch_character = !switch_character
		$AudioStreamPlayer.stream = load("res://SFX/spell1_0.wav")
		$AudioStreamPlayer.play()
		
	
func move():
	# moving left and right
	if Input.is_action_pressed("left") and Input.is_action_pressed("right"):
		motion.x = 0
	elif Input.is_action_pressed("left"):
		motion.x = -SPEED
	elif Input.is_action_pressed("right"):
		motion.x = SPEED
	else:
		motion.x = 0
	
func apply_gravity():
	if position.y > WORLD_LIMIT:
		death()
	if is_on_floor() and motion.y > 0:
		motion.y = 0 
	# fix the bug when hitting the ceiling and hovering
	# by setting the motion to be y = 1, gravity kicks in
	elif is_on_ceiling():
		motion.y = 1
	else:
		motion.y += GRAVITY
	
func jump():
	if Input.is_action_pressed("jump") and is_on_floor():
		motion.y -= JUMP_SPEED
		$AudioStreamPlayer.stream = load("res://SFX/jump1.ogg")
		$AudioStreamPlayer.play()

func animate():
	var animation_alt = ""
	if switch_character:
		animation_alt += "_alt"
	
	
	if motion.y < 0:
		$PlayerAnimation.play("jump" + animation_alt)
	elif motion.x > 0:
		$PlayerAnimation.play("walk" + animation_alt)
		$PlayerAnimation.flip_h = false
	elif motion.x < 0:
		$PlayerAnimation.play("walk" + animation_alt)
		$PlayerAnimation.flip_h = true
	else:
		$PlayerAnimation.play("idle" + animation_alt)
		
func death():
	# jump in a shocked way
	position.y -= 1
	yield(get_tree(), "idle_frame")
	motion.y = -JUMP_SPEED
	lives -= 1
	$AudioStreamPlayer.stream = load("res://SFX/pain.ogg")
	$AudioStreamPlayer.play()
	if lives == 0:
		get_tree().change_scene("res://Levels/Level1.tscn")
		
		
