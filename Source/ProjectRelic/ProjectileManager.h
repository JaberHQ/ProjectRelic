// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "ProjectileManager.generated.h"

//UENUM( BlueprintType )
//enum class Type:uint8
//{
//	AssaultRifle UMETA(DisplayName = "AssaultRifle"),
//	Pistol UMETA( DisplayName = "Pistol" ),
//	Fireball UMETA( DisplayName = "Fireball" )
//};


/*******************************************************************************************************************************************************************
 * Type: Class
 *
 * Name: ProjectileManager
 *
 * Author: Jaber Ahmed
 *
 * Purpose: Projectile manager for characters to shoot (Prototype)
 *
 * Functions: AProjectileManager, ~AProjectileManager, virtual void BeginPlay() override, virtual void Tick( float deltaTime ) override,
 *			  void OnHit( UPrimitiveComponent* hitComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, FVector normalImpulse, const FHitResult& hit ),
 *			  void ShootInDirection( const FVector& shootDirection )
 *
 * References: https://docs.unrealengine.com/4.26/en-US/ProgrammingAndScripting/ProgrammingWithCPP/CPPTutorials/FirstPersonShooter/3/
 *
 * See Also: CharacterManager
 *
 * Change Log:
 * Date          Initials    Version     Comments
 * 24/04/2022    JA			 v1.0		 Created an actor for projectile shooting
 * 30/05/2022    JA			 v1.1		 Depreciated, see CPP_Projectile	
 *******************************************************************************************************************************************************************/
UCLASS()
class PROJECTRELIC_API AProjectileManager : public AActor
{
	GENERATED_BODY()

private:
	bool m_isHit; // Is hit
	float m_sphereRadius; // Radius of sphere
	float m_initialSpeed; // Initial projectile speed
	float m_maxSpeed; // Max projectile speed
	float m_bounciness; // Projectile bounciness
	float m_projectileGravityScale; // Porjectile gravity scale
	float m_impulse; // Impulse multiplier

public:	
	UPROPERTY( VisibleDefaultsOnly, Category = Projectile )
	USphereComponent* collisionComponent; // Collision component

	UPROPERTY( VisibleAnywhere, Category = Movement )
	UProjectileMovementComponent* projectileMovementComponent; // Movement

	UPROPERTY( VisibleDefaultsOnly, Category = Projectile )
		UStaticMeshComponent* projectileMeshComponent; // Projectile Mesh

	UPROPERTY( VisibleDefaultsOnly, Category = Movement )
		UMaterialInstanceDynamic* projectileMaterialInstance; // Projectile Material

public:
	/********************************************************
	   *   Function        : AProjectileManager()
	   *   Purpose         : Constructor
	   *   Parameters      : N/A
	   *   Returns         : N/A
	   *   Date altered    : 09/04/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	*********************************************************/
	AProjectileManager();
	/********************************************************
	   *   Function        : virtual void BeginPlay() override
	   *   Purpose         : On start on play
	   *   Parameters      : N/A
	   *   Returns         : N/A
	   *   Date altered    : 09/04/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	*********************************************************/
	virtual void BeginPlay() override;
	/*******************************************************************
	   *   Function        : virtual void Tick(float deltaTime) override
	   *   Purpose         : Every tick
	   *   Parameters      : float deltaTime
	   *   Returns         : N/A
	   *   Date altered    : 09/04/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	*******************************************************************/
	// Called every frame
	virtual void Tick( float deltaTime ) override;
	/******************************************************************************************************************************************************************************
	   *   Function        : void OnHit( UPrimitiveComponent* hitComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, FVector normalImpulse, const FHitResult& hit )
	   *   Purpose         : When projectile hits something
	   *   Parameters      : UPrimitiveComponent* hitComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, FVector normalImpulse, const FHitResult& hit
	   *   Returns         : N/A
	   *   Date altered    : 09/04/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	******************************************************************************************************************************************************************************/
	UFUNCTION()
		void OnHit( UPrimitiveComponent* hitComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, FVector normalImpulse, const FHitResult& hit );
	/******************************************************************************
	   *   Function        : void ShootInDirection( const FVector& shootDirection )
	   *   Purpose         : Initalise the projectile's velocity 
	   *   Parameters      : const FVector& shootDirection 
	   *   Returns         : N/A
	   *   Date altered    : 09/04/2022
	   *   Contributors    : Jaber Ahmed
	   *   Notes           : N/A
	   *   See also        : N/A
	******************************************************************************/
	void ShootInDirection( const FVector& shootDirection );
};
