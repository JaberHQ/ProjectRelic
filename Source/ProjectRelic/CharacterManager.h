// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ProjectileManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CharacterManager.generated.h"

/***************************************************************************************
 * Type: Class
 *
 * Name: CharacterManager
 *
 * Author: Jaber Ahmed
 *
 * Purpose: Parent class for characters
 *
 * Functions: Shoot(), MoveForward(), MoveRight(), BeginSprint(), EndSprint(),
 * BeginCrouch(), EndCrouch(), AimIn(), AimOut(),
 *
 * References:
 *
 * See Also:
 *
 * Change Log:
 * Date          Initials    Version     Comments
 * ----------    --------    -------     ----------------------------------------------
 ***************************************************************************************/

UCLASS()
class PROJECTRELIC_API ACharacterManager : public ACharacter
{
	GENERATED_BODY()

private:
	bool m_holdADS;

public:
	// Sets default values for this character's properties
	ACharacterManager();

	// Fire projectile
	UFUNCTION()
		void Shoot();

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Gameplay )
		FVector muzzleOffset;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Spring Arm Component to follow the camera behind the player
	UPROPERTY( VisibleAnywhere, BlueprintReadWrite )
		class USpringArmComponent* springArmComp;

	// Player follow camera
	UPROPERTY( VisibleAnywhere, BlueprintReadWrite )
		class UCameraComponent* cameraComp;

	// Player ADS camera
	UPROPERTY( VisibleAnywhere, BlueprintReadWrite )
		class UCameraComponent* aDSCameraComp;

	// Gun
	UPROPERTY( VisibleAnywhere, BlueprintReadWrite )
		class USkeletalMeshComponent* gunComp;

	UPROPERTY( EditDefaultsOnly, Category = Projectile )
		TSubclassOf<class AProjectileManager> projectileClass;

	// Input 
	void MoveForward( float inputAxis );
	void MoveRight( float inputAxis );

	// Sprint
	void BeginSprint();
	void EndSprint();

	// Crouch
	void BeginCrouch();
	void EndCrouch();

	// Aim
	void AimIn();
	void AimOut();

	void SetHoldADS( bool holdADS );
	bool GetHoldADS();

public:
	// Called every frame
	virtual void Tick( float deltaTime ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent( class UInputComponent* playerInputComponent ) override;


};
