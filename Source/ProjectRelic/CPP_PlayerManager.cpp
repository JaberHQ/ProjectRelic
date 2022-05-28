// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_PlayerManager.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "CPP_AIController.h"
#include "Animation/AnimInstance.h"
#include "CPP_AIManager.h"

ACPP_PlayerManager::ACPP_PlayerManager()
	:m_canTakedown( true )
	,m_takedownTraceDistance( 250.0f )
{
}

void ACPP_PlayerManager::BeginPlay()
{
	Super::BeginPlay();

	// Start player off as crouching
	Crouch();
}

void ACPP_PlayerManager::SetupPlayerInputComponent( UInputComponent* PlayerInputComponent )
{
	Super::SetupPlayerInputComponent( PlayerInputComponent );

	PlayerInputComponent->BindAction( "MeleeTakedown", IE_Pressed, this, &ACPP_PlayerManager::Takedown );
}

void ACPP_PlayerManager::SetCanTakedown( bool canTakedown )
{
	
	m_canTakedown = canTakedown;
	
}

bool ACPP_PlayerManager::GetCanTakedown()
{
	return m_canTakedown;
}

void ACPP_PlayerManager::Takedown()
{
	if( m_canTakedown )
	{
		TraceForward_Implementation();
	}
}

void ACPP_PlayerManager::TraceForward_Implementation()
{
	FVector location; // Location
	FRotator rotation; // Rotation
	FHitResult hit; // Hit
	FCollisionQueryParams traceParams; // Trace parameters

	GetController()->GetPlayerViewPoint( location, rotation );

	// Set start and end
	FVector start = location;
	FVector end = ( GetActorLocation() + ( GetActorForwardVector() * m_takedownTraceDistance ) );

	// Line trace
	bool bHit = GetWorld()->LineTraceSingleByChannel( hit, start, end, ECC_Camera, traceParams );

	// Draw a line for debug
	DrawDebugLine( GetWorld(), start, end, FColor::Red, false, 5.0f );

	// If line has hit
	if( bHit )
	{
		// Box where collision has occured
		DrawDebugBox( GetWorld(), hit.ImpactPoint, FVector( 5, 5, 5 ), FColor::Emerald, false, 2.0f );

		// Get AI controller
		ACPP_AIManager* managerAI = Cast<ACPP_AIManager>( hit.GetActor() );
		if( managerAI )
		{
			//Spring arm stuff---

			GetCharacterMovement()->DisableMovement();
			m_canTakedown = false;

			// Meant to be new transform rotation, not actor rotation
			// SetActorTransform( managerAI->GetActorTransform() );	
			SetActorRotation( managerAI->GetActorRotation() );
			SetActorLocation( ( managerAI->GetActorLocation() ) + ( managerAI->GetActorForwardVector() * -175.0f ) );
			if( animTakedown )
			{
				PlayAnimMontage( animTakedown, 1.0f, NAME_None );
			}
			//PlayAnimMontage( animTakedown );
			//GetMesh()->PlayAnimation( animTakedown, false );
			
			//managerAI->GetMesh()->PlayAnimation( animTakedown, false );
			managerAI->Takedown();

			//Add float to return value (length of montage)
			//delay
			//SetActorLocation( GetActorLocation() + GetActorForwardVector() * 175.0f );
			//GetCharacterMovement()->SetMovementMode( EMovementMode::MOVE_Walking );

			// --- Move back camera 
		}
	}
}
