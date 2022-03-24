// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterManager.h"
#include "ProjectileManager.h"
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
	
	// Fire projectile
	UFUNCTION()
		void Shoot();

	UPROPERTY( EditAnywhere, BlueprintReadWrite, Category = Gameplay )
		FVector MuzzleOffset;

protected:
	// Spring Arm Component to follow the camera behind the player
	UPROPERTY( VisibleAnywhere, BlueprintReadWrite )
		class USpringArmComponent* SpringArmComp;

	// Player follow camera
	UPROPERTY( VisibleAnywhere, BlueprintReadWrite )
		class UCameraComponent* CameraComp;

	// Player ADS camera
	UPROPERTY( VisibleAnywhere, BlueprintReadWrite )
		class UCameraComponent* ADSCameraComp;

	// Gun
	UPROPERTY( VisibleAnywhere, BlueprintReadWrite )
		class USkeletalMeshComponent* GunComp;

	UPROPERTY( EditDefaultsOnly, Category = Projectile )
		TSubclassOf<class AProjectileManager> ProjectileClass;

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

	
};
