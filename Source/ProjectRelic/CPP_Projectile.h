// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "CPP_Projectile.generated.h"

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
 * 30/05/2022	 JA			 v2.0        Now using raycast bullets
 *******************************************************************************************************************************************************************/
UCLASS()
class PROJECTRELIC_API ACPP_Projectile : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ACPP_Projectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
