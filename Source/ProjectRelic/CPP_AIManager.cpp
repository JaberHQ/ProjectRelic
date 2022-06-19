// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_AIManager.h"
#include "CPP_AIController.h"
#include "Engine/World.h"
#include "CPP_PlayerManager.h"
#include "Kismet/KismetMathLibrary.h"
#include "Misc/App.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

ACPP_AIManager::ACPP_AIManager()
	:health( 100.0f )
	,defaultHealth( 100.0f )
	,m_sightRadius( 1000.0f )
	,m_loseSightRadius( 1020.0f )
	,m_peripheralVisionAngleDegrees( 35.0f )
	,m_patrolSpeed( 300.0f )
	,m_chaseSpeed( 600.0f )
	,m_shotDamage( 23.3f )
	,m_deathTimer( 25.0f )
	,m_detectionCount( 0.0f )
	,m_hasBeenSeen( false )
	,m_sightValuePercent( 0.0f )
{
	// Initialise components
	perceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>( TEXT( "AIPerceptionComponent" ) );
	sightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>( TEXT( "SightConfig" ) );
	boxComponent = CreateDefaultSubobject<UBoxComponent>( TEXT( "TakedownBox" ) );

	// Perception config
	perceptionComp->ConfigureSense( *sightConfig );
	perceptionComp->SetDominantSense( sightConfig->GetSenseImplementation() );

	// Sight config
	sightConfig->SightRadius = m_sightRadius;
	sightConfig->LoseSightRadius = m_loseSightRadius;
	sightConfig->PeripheralVisionAngleDegrees = m_peripheralVisionAngleDegrees;
	sightConfig->DetectionByAffiliation.bDetectEnemies = true;
	sightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	sightConfig->DetectionByAffiliation.bDetectFriendlies = false;

	
}

void ACPP_AIManager::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	//----- IMPLEMENTATION NEEDED -----
	//LINK THE PROGRESS BAR TO THIS CODE FOR DETECTION SYSTEM
	m_sightValuePercent = UKismetMathLibrary::FInterpTo_Constant( m_sightValuePercent, UKismetMathLibrary::SelectFloat( 1.0f, 0.0f, m_hasBeenSeen ), 
							FApp::GetDeltaTime(), m_detectionCount );
	// set m_sightValuePercent = ProgressBarPercent;
}

void ACPP_AIManager::BeginPlay()
{
	Super::BeginPlay();

	// If enemy 'senses' the player
	perceptionComp->OnPerceptionUpdated.AddDynamic( this, &ACPP_AIManager::OnPlayerCaught );
	boxComponent->OnComponentBeginOverlap.AddDynamic( this, &ACPP_AIManager::OnBoxBeginOverlap );
	boxComponent->OnComponentEndOverlap.AddDynamic( this, &ACPP_AIManager::OnBoxEndOverlap );
}

ACPP_PatrolPoint* ACPP_AIManager::GetPatrolPath()
{
	return m_patrolPath;
}

void ACPP_AIManager::OnBoxBeginOverlap( UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult )
{
	// Player reference
	ACPP_PlayerManager* playerManager = Cast<ACPP_PlayerManager>( UGameplayStatics::GetPlayerPawn( GetWorld(), 0 ) );

	// Set bool to takedown
	playerManager->SetCanTakedown( true );
}

void ACPP_AIManager::OnBoxEndOverlap( UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex )
{
	ACPP_PlayerManager* playerManager = Cast<ACPP_PlayerManager>( UGameplayStatics::GetPlayerPawn( GetWorld(), 0 ) );
}

void ACPP_AIManager::Takedown()
{
	// Disable movement
	GetCharacterMovement()->DisableMovement();

	//Unpossess
	UnPossessed();

	// Set collision
	SetActorEnableCollision( false );
	
	// Play animation
	if( animTakedownAI )
	{
		PlayAnimMontage( animTakedownAI );
	}

	// Delay then death
	FTimerHandle deathDelayTimer;
	GetWorld()->GetTimerManager().SetTimer( deathDelayTimer, this, &ACPP_AIManager::DelayDeath, m_deathTimer, false );
}

void ACPP_AIManager::DelayDeath()
{
	// Destroy Actor
	Destroy();
}

void ACPP_AIManager::TakeAttack()
{

	// If health remains, decrease health else pronounce Enemy Dead
	health >= 0.0f ? health -= m_shotDamage : Destroy(); // Change to death animation
	
	// Debug to show health
	FString healthDebug = FString::SanitizeFloat( health );
	GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Blue, healthDebug ) ;
}



void ACPP_AIManager::OnPlayerCaught( const TArray<AActor*>& caughtActors )
{
	// AI Controller reference
	ACPP_AIController* controllerAI = Cast<ACPP_AIController>( GetController() );

	// Player reference
	ACPP_PlayerManager* playerManager = Cast<ACPP_PlayerManager>( UGameplayStatics::GetPlayerPawn( GetWorld(), 0 ) );

	if( controllerAI )
	{
		if( playerManager )
		{
			// Sight config
			UAIPerceptionSystem::RegisterPerceptionStimuliSource( this, sightConfig->GetSenseImplementation(), controllerAI );

			// Get location
			FVector playerLocation = perceptionComp->GetActorInfo( *caughtActors[ 0 ] )->GetStimulusLocation( sightConfig->GetSenseID() );
			FVector enemyLocation = perceptionComp->GetActorInfo( *caughtActors[ 0 ] )->GetReceiverLocation( sightConfig->GetSenseID() );
			float distance = FVector::Distance( playerLocation, enemyLocation );
			m_detectionCount = UKismetMathLibrary::NormalizeToRange( distance, 0.0f, 500.0f );

			// Debug
			FString distanceDebug = FString::SanitizeFloat( m_sightValuePercent );
			GEngine->AddOnScreenDebugMessage( -1, 5.f, FColor::Red, distanceDebug );

			m_hasBeenSeen = true;

			if( m_sightValuePercent <= 0.0f && !m_hasBeenSeen )
			{
				//-------- IMPLEMENTATION NEEDED --------
				//Lost Player
			}
			else if ( m_sightValuePercent == 1.0f )
			{

				// Debug message
				GEngine->AddOnScreenDebugMessage( -1, 5.0f, FColor::Red, ( TEXT( "Caught" ) ) );

				// Set actor (Player) as caught
				controllerAI->SetPlayerCaught( caughtActors );


				// Find the distance between the two
				float distanceToPlayer = FVector::Distance( playerLocation, enemyLocation );

				m_aimingIn = true;

				// Shoot towards Player
				//StartShooting();

				// Get float value (curve) && Set detection speed = float value
				//m_detectionSpeed = myCurve->GetFloatValue( distanceToPlayer );
			}
			
		}
	}
}

void ACPP_AIManager::SightDetectionDelegate()
{
	sightDetectionD.Broadcast( m_sightValuePercent );
}