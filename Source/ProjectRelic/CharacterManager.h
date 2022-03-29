// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ProjectileManager.h"
#include "CharacterManager.generated.h"

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
		FVector MuzzleOffset;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

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

public:
	// Called every frame
	virtual void Tick( float DeltaTime ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent( class UInputComponent* PlayerInputComponent ) override;

	// Input 
	void MoveForward( float InputAxis );
	void MoveRight( float InputAxis );

	// Sprint
	void BeginSprint();
	void EndSprint();

	// Crouch
	void BeginCrouch();
	void EndCrouch();

	// Aim
	void AimIn();
	void AimOut();

	void SetHoldADS( bool HoldADS );
	bool GetHoldADS();
};
