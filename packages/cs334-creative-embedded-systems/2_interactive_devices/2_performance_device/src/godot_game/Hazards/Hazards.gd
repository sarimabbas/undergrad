extends Area2D

func _ready():
	pass


func _on_SpikeTop_body_entered(body):
	body.death()
