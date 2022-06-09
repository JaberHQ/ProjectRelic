// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_PlayerManager.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "CPP_AIController.h"
#include "Animation/AnimInstance.h"
#include "CPP_AIManager.h"

ACPP_PlayerManager::ACPP_PlayerManager()
	:m_canTakedown( true )
	,takedownTraceDistance( 250.0f )
	,health( 200.0f )
	,defaultHealth( 200.0f )
	,m_shotDamage( 10.0f )
	,m_animPosition( 40.0f )
	,m_animCompletion( 5.0f )
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
	PlayerInputComponent->BindAction( "Shoot", IE_Pressed, this, &ACPP_CharacterManager::StartShooting );
	PlayerInputComponent->BindAction( "Shoot", IE_Released, this, &ACPP_CharacterManager::StopShooting );
	PlayerInputComponent->BindAction( "Aim", IE_Pressed, this, &ACPP_CharacterManager::StartAim );
	PlayerInputComponent->BindAction( "Aim", IE_Released, this, &ACPP_CharacterManager::StopAim );
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
	FVector end = ( GetActorLocation() + ( GetActorForwardVector() * takedownTraceDistance ) );

	// Line trace
	bool bHit = GetWorld()->LineTraceSingleByChannel( hit, start, end, ECC_WorldDynamic, traceParams );

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
			// Debug
			GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Red, hit.GetActor()->GetName() );
			
			// --- Spring arm stuff ---

			// Disable character movement
			GetCharacterMovement()->DisableMovement();

			// Disable Player input
			APlayerController* playerController = GetWorld()->GetFirstPlayerController();
			DisableInput( playerController );

			// Set bool
			m_canTakedown = false;

			// Set actor transform
			SetActorRotation( managerAI->GetActorRotation() ); // Meant to be new transform rotation, not actor rotation
			SetActorLocation( ( managerAI->GetActorLocation() ) + ( managerAI->GetActorForwardVector() * -m_animPosition ) );
			
			if( animTakedown )
			{
				// Play animation
				PlayAnimMontage( animTakedown );
				
				// Takedown AI
				managerAI->Takedown();

				// Delay 
				FTimerHandle delayTimer;
				GetWorld()->GetTimerManager().SetTimer( delayTimer, this, &ACPP_PlayerManager::AnimationExecuted, m_animCompletion, false );		
			}
		}
	}
}

void ACPP_PlayerManager::AnimationExecuted()
{
	// Return character back to position
	//SetActorLocation( GetActorLocation() + GetActorForwardVector() * m_animPosition );

	// Enable Player input
	APlayerController* playerController = GetWorld()->GetFirstPlayerController();
	EnableInput( playerController );

	// Re-enable movement
	GetCharacterMovement()->SetMovementMode( EMovementMode::MOVE_Walking );

	// --- Move back camera ---

	// Player can takedown AI again
	m_canTakedown = true;
}

void ACPP_PlayerManager::TakeAttack()
{
	// If health remains, decrease health else pronounce Player as dead
	health > 0.0f ? health -= m_shotDamage : GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Red, "Dead" );

	// Debug
	FString healthDebug = FString::SanitizeFloat( health );
	GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Red, healthDebug );
}


