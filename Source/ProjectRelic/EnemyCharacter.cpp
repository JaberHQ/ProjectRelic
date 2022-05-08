// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyCharacter.h"
#include "EnemyController.h"
#include "PlayerCharacter.h"

AEnemyCharacter::AEnemyCharacter()
	:m_health( 100.0f )
	,m_detectionTimer( 0.0f )	
	,m_sightRadius( 1000.0f )
	,m_loseSightRadius( 1020.0f )
	,m_peripheralVisionAngleDegrees( 35.0f )
	,m_patrolSpeed( 300.0f )
	,m_chaseSpeed( 600.0f )
	,m_canTakedown( true )
	,m_hasBeenSeen( false )
{
	// Set default walk speed
	UpdateWalkSpeed( m_patrolSpeed );

	// Initialise components
	perceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>( TEXT( "AIPerception Component" ) );
	sightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>( TEXT( "Sight Config" ) );
	widgetComp = CreateDefaultSubobject<UWidgetComponent>( TEXT( "Widget Component" ) );

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

	// Attatch widget
	widgetComp->SetupAttachment( RootComponent );
}

AEnemyCharacter::~AEnemyCharacter()
{	
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// If enemy 'senses' the player
	perceptionComp->OnPerceptionUpdated.AddDynamic( this, &AEnemyCharacter::OnPlayerCaught );
}

bool AEnemyCharacter::CanTakedown()
{
	return m_canTakedown;
}

void AEnemyCharacter::Interact()
{
	UE_LOG( LogTemp, Warning, TEXT( "Implementation" ) );
}

void AEnemyCharacter::SetHasBeenSeen( bool hasBeenSeen )
{
	m_hasBeenSeen = hasBeenSeen;
}

bool AEnemyCharacter::GetHasBeenSeen() const
{
	return m_hasBeenSeen;
}

float AEnemyCharacter::GetDetectionSpeed() const
{
	return m_detectionSpeed;
}

void AEnemyCharacter::OnPlayerCaught( const TArray<AActor*>& caughtActors )
{
	// Pointer to Enemy controller
	AEnemyController* enemyController = Cast<AEnemyController>( GetController() );

	// Pointer to Player character
	APlayerCharacter* playerCharacter = Cast<APlayerCharacter>( UGameplayStatics::GetPlayerPawn(GetWorld(), 0 ));

	if( enemyController )
	{	
		if( playerCharacter )
		{
			// If player is not invisible
			if( playerCharacter->GetInvisible() == false )
			{
				// Debug message
				GEngine->AddOnScreenDebugMessage( -1, 5.0f, FColor::Red, ( TEXT( "Caught" ) ) );

				// Set actor (player) as caught
				enemyController->SetPlayerCaught( caughtActors );

				// Shoot at player
				Shoot();

				// //Sight config
				UAIPerceptionSystem::RegisterPerceptionStimuliSource( this, sightConfig->GetSenseImplementation(), enemyController );

				//Speed up enemy to sprint
				UpdateWalkSpeed( m_chaseSpeed );

				// Get location
				FVector playerLocation = perceptionComp->GetActorInfo( *caughtActors[ 0 ] )->GetStimulusLocation( sightConfig->GetSenseID() );
				FVector enemyLocation = perceptionComp->GetActorInfo( *caughtActors[ 0 ] )->GetReceiverLocation( sightConfig->GetSenseID() );

				// Find the distance between the two
				float distanceToPlayer = FVector::Distance( playerLocation, enemyLocation );

				// Normalize to range
				// -- NEEDS IMPLEMENTATION --

				// Get float value (curve) && Set detection speed = float value
				m_detectionSpeed = myCurve->GetFloatValue( distanceToPlayer );

				// Set bool to true
				m_hasBeenSeen = true;

				// Broadcast delegate
				SightDetectionDelegate();
			}
		}		
	}		
}

void AEnemyCharacter::SightDetectionDelegate()
{
	// Broadcast delegate variables
	sightRegisteredD.Broadcast( m_hasBeenSeen, m_detectionSpeed );
}



