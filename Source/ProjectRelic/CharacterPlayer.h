// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterManager.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "CharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTRELIC_API ACharacterPlayer : public ACharacterManager
{
	GENERATED_BODY()

private:
	UPROPERTY( EditAnywhere, Category = "Camera" )
		UCameraComponent* camera;

protected:
	// Input 
	void moveForward( float inputAxis );
	void moveRight( float inputAxis );
	void turn( float inputAxis );
	void lookUp( float inputAxis );

	// Sprint
	void beginSprint();
	void endSprint();

	// Crouch
	void beginCrouch();
	void endCrouch();

public:
	ACharacterPlayer();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent( class UInputComponent* PlayerInputComponent ) override;
};
