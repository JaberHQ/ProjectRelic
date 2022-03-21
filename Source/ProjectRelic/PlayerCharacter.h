// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterManager.h"
#include "PlayerCharacter.generated.h"

/**
 *
 */
UCLASS()
class PROJECTRELIC_API APlayerCharacter : public ACharacterManager
{
	GENERATED_BODY()

private:
	bool m_holdADS;
	
public:
	APlayerCharacter();
	virtual void SetupPlayerInputComponent( class UInputComponent* PlayerInputComponent ) override;
	
	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Gameplay )
	bool isAimedin;

protected:
	// Spring Arm Component to follow the camera behind the player
	UPROPERTY( VisibleAnywhere, BlueprintReadWrite )
		class USpringArmComponent* SpringArmComp;

	// Player follow camera
	UPROPERTY( VisibleAnywhere, BlueprintReadWrite )
		class UCameraComponent* CameraComp;

	// Input 
	void moveForward( float inputAxis );
	void moveRight( float inputAxis );

	// Sprint
	void beginSprint();
	void endSprint();

	// Crouch
	void beginCrouch();
	void endCrouch();

	// Aim
	void aimIn();
	void aimOut();


	void setHoldADS( bool holdADS );
	bool getHoldADS();

};
