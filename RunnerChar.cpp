#include "RunnerChar.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "KillObstacle.h"
#include "KillWall.h"
#include "Engine.h"

// Sets default values
ARunnerChar::ARunnerChar()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Setup Capsule Size & Collision
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);

	//Controller will not be handling rotation of Player
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	SideViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Side View Camera"));

	//Controller will not be handling rotation of Camera
	SideViewCamera->bUsePawnControlRotation = false;

	//Setup Character Movement to auto rotate with movement direction
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.f, 0.0f);

	//Setup Character Physics & Movement
	GetCharacterMovement()->GravityScale = 2.0f;
	GetCharacterMovement()->AirControl = 0.8f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.0f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MaxFlySpeed = 500.f;

	//Store Start Location & Setup Z
	tempPos = GetActorLocation();
	zPosition = tempPos.Z + 300.f;

}

// Called when the game starts or when spawned
void ARunnerChar::BeginPlay()
{
	Super::BeginPlay();

	//Set Capsule Component to run custom function on overlap
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ARunnerChar::OnOverlapBegin);

	//Set start position for score calculation
	startPos = GetActorLocation();

	canMove = true;
	
}

// Called every frame
void ARunnerChar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Use tick to have Camera follow Player
	tempPos = GetActorLocation();
	tempPos.X -= 850.f;
	tempPos.Z = zPosition;
	SideViewCamera->SetWorldLocation(tempPos);

	//Update score as player moves forward, but don't reduce score for backward movement
	if (canMove)
	{
		if (distanceScore <= FMath::Abs(FMath::RoundFromZero((startPos.Y - GetActorLocation().Y) / 10)))
		{
			distanceScore = FMath::Abs(FMath::RoundFromZero((startPos.Y - GetActorLocation().Y) / 10));
		}
	}
}

// Called to bind functionality to input
void ARunnerChar::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("Move", this, &ARunnerChar::Move);
}

void ARunnerChar::Move(float value)
{
	if (canMove)
		AddMovementInput(FVector(0.f, 1.f, 0.f), value);
}

void ARunnerChar::RestartLevel_Implementation()
{
	UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()));
}

void ARunnerChar::PlayerDeath_Implementation()
{
	//Used for Blueprint override of PlayerDeath function
}

void ARunnerChar::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr)
	{
		//Try cast to either KillObstacle or KillWall to decide whether to kill Player & restart level
		AKillObstacle* KillerObstacle = Cast<AKillObstacle>(OtherActor);
		AKillWall* KillerWall = Cast<AKillWall>(OtherActor);

		if (KillerObstacle || KillerWall)
		{
			GetMesh()->Deactivate();
			GetMesh()->SetVisibility(false);

			canMove = false;

			ARunnerChar::PlayerDeath();

			FTimerHandle UnusedHandle;
			GetWorldTimerManager().SetTimer(UnusedHandle, this, &ARunnerChar::RestartLevel, 3.f, false);
		}
	}

}

