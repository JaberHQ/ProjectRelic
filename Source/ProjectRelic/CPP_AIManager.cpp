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
	,m_loseSightRadius( 1500.0f )
	,m_peripheralVisionAngleDegrees( 35.0f )
	,m_patrolSpeed( 300.0f )
	,m_chaseSpeed( 600.0f )
	,m_shotDamage( 23.3f )
	,m_deathTimer( 25.0f )
	,m_hasBeenSeen( false )
	,m_hasBeenCaught( false )
	,m_sightValuePercent( 0.0f )
	,m_curveFloat( 0.0f )
	,m_hasSeenSomething( false )
	,m_detectionSpeed( 0.0f )
	,m_headShotDamage( 2.0f )
	,soundHuh()
	,m_isInCover( false )
{
	// Initialise components
	perceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>( TEXT( "AIPerceptionComponent" ) );
	sightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>( TEXT( "SightConfig" ) );
	hearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>( TEXT( "HearingConfig" ) );
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
	sightConfig->DetectionByAffiliation.bDetectFriendlies = true;

	// Hearing config
	hearingConfig->HearingRange = 3000.0f;
	hearingConfig->DetectionByAffiliation.bDetectEnemies = true;
	hearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
	hearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
	hearingConfig->SetMaxAge( 35.0f );

	perceptionComp->ConfigureSense( *hearingConfig );

	GetCharacterMovement()->GetNavAgentPropertiesRef().bCanCrouch = true;

}

void ACPP_AIManager::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	//m_detectionSpeed = DetectionSpeedCalculation();
	m_detectionSpeed = myCurve->GetFloatValue( m_curveFloat );

	
	m_sightValuePercent = UKismetMathLibrary::FInterpTo_Constant( m_sightValuePercent, UKismetMathLibrary::SelectFloat( 1.0f, 0.0f, m_hasSeenSomething ), 
						FApp::GetDeltaTime(), m_detectionSpeed );


	// If sight value is full
	if( m_sightValuePercent >= 1.0f )
	{
		// Set player has been caught
		m_hasBeenCaught = true;

		// Cast to AI controller
		ACPP_AIController* controllerAI = Cast<ACPP_AIController>( GetController() );

		if( controllerAI )
		{
			// Cast to player
			ACPP_PlayerManager* playerManager = Cast<ACPP_PlayerManager>( UGameplayStatics::GetPlayerPawn( GetWorld(), 0 ) );

			if( playerManager )
			{
				// Set Player and AI location
				FVector playerLocation = playerManager->GetActorLocation();
				FVector enemyLocation = controllerAI->GetPawn()->GetActorLocation();

				// Find the distance between the two
				float distance = FVector::Distance( playerLocation, enemyLocation );

				// Set curve float
				m_curveFloat = UKismetMathLibrary::NormalizeToRange( distance, 0.0f, 1000.0f );
				//controllerAI->PlayerHasShot();

				controllerAI->SetHasLineOfSight( true );
			}
		}
	}

	// Broadcast delegate for sight detection widget
	SightDetectionDelegate();
}

void ACPP_AIManager::BeginPlay()
{
	Super::BeginPlay();


	// If enemy 'senses' the player
	perceptionComp->OnPerceptionUpdated.AddDynamic( this, &ACPP_AIManager::OnPlayerCaught );

	// Box component overlap
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
	// -- IMPLEMENTATION NEEDED --
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
	// Temporary damage variable
	float damage = m_shotDamage;

	// If enemy has been shot in the head
	if( m_shotInHead )
	{
		// Increase damage value
		damage = m_shotDamage * m_headShotDamage;
	}
	
	// If health remains, decrease health else pronounce Enemy Dead
	health >= 0.0f ? health -= damage : Destroy(); // Change to death animation
	
	// AI Controller reference
	ACPP_AIController* controllerAI = Cast<ACPP_AIController>( GetController() );

	// Player reference
	ACPP_PlayerManager* playerManager = Cast<ACPP_PlayerManager>( UGameplayStatics::GetPlayerPawn( GetWorld(), 0 ) );

	if( controllerAI )
	{
		// Set player caught
		controllerAI->PlayerHasShot();
		m_sightValuePercent = 1.0f;
		if( playerManager )
		{
			if( health <= 0 )
			{
				playerManager->SetHitmarkerActive( true );
			}
		}
	}

	// Enemy has been shot
	m_hasBeenShot = true;

	// Reset shot to the head bool
	m_shotInHead = false;
}

float ACPP_AIManager::DetectionSpeedCalculation()
{
	// AI Controller reference
	ACPP_AIController* controllerAI = Cast<ACPP_AIController>( GetController() );

	// Player reference
	ACPP_PlayerManager* playerManager = Cast<ACPP_PlayerManager>( UGameplayStatics::GetPlayerPawn( GetWorld(), 0 ) );

	/*if( controllerAI )
	{
		if( playerManager )
		{
			FVector playerLocation = playerManager->GetActorLocation();
			FVector enemyLocation = controllerAI->GetPawn()->GetActorLocation();
			float distance = FVector::Distance( playerLocation, enemyLocation );
			float speed = UKismetMathLibrary::NormalizeToRange( distance, 0.0f, 500.0f );
			float detectionSpeed = myCurve->GetFloatValue( speed );
			return detectionSpeed;
		}
	}*/
	return 0.0f;
}

void ACPP_AIManager::DelayInvestigate()
{
}

bool ACPP_AIManager::HasCaughtPlayer()
{
	return m_hasBeenCaught;
}

void ACPP_AIManager::OnUpdated( const TArray<AActor*>& caughtActors )
{
	for( int i = 0; i < caughtActors.Num(); i++ )
	{



		// AI Controller reference
		ACPP_AIController* controllerAI = Cast<ACPP_AIController>( GetController() );

		// Player reference
		ACPP_PlayerManager* playerManager = Cast<ACPP_PlayerManager>( UGameplayStatics::GetPlayerPawn( GetWorld(), 0 ) );
		if( controllerAI )
		{
			if( playerManager )
			{
				m_hasSeenSomething = true;


				// Sight config
				UAIPerceptionSystem::RegisterPerceptionStimuliSource( this, sightConfig->GetSenseImplementation(), controllerAI );

				// Get location
				FVector playerLocation = perceptionComp->GetActorInfo( *caughtActors[ 0 ] )->GetStimulusLocation( sightConfig->GetSenseID() );
				FVector enemyLocation = perceptionComp->GetActorInfo( *caughtActors[ 0 ] )->GetReceiverLocation( sightConfig->GetSenseID() );

				// Distance between Player and Enemy
				float distance = FVector::Distance( playerLocation, enemyLocation );

				// Set last known location of the Player
				controllerAI->SetLastKnownLocation( playerLocation );

				//Investigate
				controllerAI->SetInvestigate( true );

				FActorPerceptionBlueprintInfo info;
				perceptionComp->GetActorsPerception( playerManager, info );
				for( int j = 0; j < info.LastSensedStimuli.Num(); j++ )
				{
					const FAIStimulus stim = info.LastSensedStimuli[ j ];
					if( stim.Tag == noiseTag )
					{
						m_hasBeenCaught = true;
						controllerAI->SetInvestigate( true );
						controllerAI->SetLastKnownLocation( playerLocation );
					}
					else
					{
						controllerAI->SetHasLineOfSight( true );

						if( stim.WasSuccessfullySensed() )
						{
							// Set bool to investigate
							m_hasSeenSomething = true;
							UGameplayStatics::PlaySoundAtLocation( GetWorld(), soundHuh, GetActorLocation(), 0.3f );
						}
						else
						{
							// Go back to regular pathing
							if( m_hasBeenCaught == false )
							{
								m_hasSeenSomething = false;
							}
						}
					}
				}

				// If player has been caught
				if( m_hasBeenCaught )
				{
					// Set actor (Player) as caught
					controllerAI->PlayerHasShot();
				}
			}
		}
		
	}
}

void ACPP_AIManager::SetupPerceptionSystem()
{
}

void ACPP_AIManager::EnterCover()
{
	m_isInCover = true;
	Crouch();
	// AI Controller reference
	ACPP_AIController* controllerAI = Cast<ACPP_AIController>( GetController() );
	if( controllerAI )
	{
		
	}
}

void ACPP_AIManager::TimeToShoot()
{
	FTimerHandle m_shootTimer;
	GetWorld()->GetTimerManager().SetTimer( m_shootTimer, this, &ACPP_PlayerManager::StopShooting, 5.0f, true );

	Crouch();
}

void ACPP_AIManager::SetHasCaughtPlayer( bool boolean )
{
	m_hasBeenCaught = boolean;
}

void ACPP_AIManager::SetMaxWalkSpeed( float speed )
{
	GetCharacterMovement()->MaxWalkSpeed = speed;
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
			if( playerManager->GetInvisibilityStatus() == false )
			{
				// Set bool to investigate
				m_hasSeenSomething = true;

				// Sight config
				UAIPerceptionSystem::RegisterPerceptionStimuliSource( this, sightConfig->GetSenseImplementation(), controllerAI );

				// Get location
				FVector playerLocation = perceptionComp->GetActorInfo( *caughtActors[ 0 ] )->GetStimulusLocation( sightConfig->GetSenseID() );
				FVector enemyLocation = perceptionComp->GetActorInfo( *caughtActors[ 0 ] )->GetReceiverLocation( sightConfig->GetSenseID() );

				// Distance between Player and Enemy
				float distance = FVector::Distance( playerLocation, enemyLocation );

				// Set last known location of the Player
				controllerAI->SetLastKnownLocation( playerLocation );

				//Investigate
				controllerAI->SetInvestigate( true );

				// Actor perception
				FActorPerceptionBlueprintInfo info;
				perceptionComp->GetActorsPerception( playerManager, info );

				// If sensed
				if( info.LastSensedStimuli.Num() > 0 )
				{
					FAIStimulus stimulus = info.LastSensedStimuli[ 0 ];
				
					if( stimulus.WasSuccessfullySensed() )
					{
						// Set bool to investigate
						m_hasSeenSomething = true;
						UGameplayStatics::PlaySoundAtLocation( GetWorld(), soundHuh, GetActorLocation(), 0.3f );
					}
					else
					{
						// Go back to regular pathing
						if( m_hasBeenCaught == false )
						{
							m_hasSeenSomething = false;
						}
					}
				}
			}
			
			// If player has been caught
			if( m_hasBeenCaught )
			{
				// Set actor (Player) as caught
				controllerAI->PlayerHasShot();
			}	
		}
	}
}

void ACPP_AIManager::SightDetectionDelegate()
{
	// Broadcast delegate
	sightDetectionD.Broadcast( m_sightValuePercent, m_hasSeenSomething );
}

void ACPP_AIManager::EvaluateSightDetection()
{
	// If detection meter is empty
	if( m_sightValuePercent <= 0.0f && !m_hasSeenSomething )
	{
		// Lose player and go back to regular pathing
		GiveUp();
		LostPlayer();
	}
	else
	{
		if( m_sightValuePercent >= 1.0f )
		{
			// Attack player
			SeenPlayer();
		}
	}
	
}

void ACPP_AIManager::GiveUp()
{
}

void ACPP_AIManager::SeenPlayer()
{
}

void ACPP_AIManager::LostPlayer()
{
}




