//inclduing necessary header files.
#include "A_GameCharacter.h"
#include "AGun.h"
#include "Components/CapsuleComponent.h"
#include "TPShooterGameModeBase.h"

// Sets default values
AA_GameCharacter::AA_GameCharacter()
{
 	// Set this character to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AA_GameCharacter::BeginPlay()
{
	Super::BeginPlay();
	//setting up gun and health
	Health = MaxHealth;
	Gun = GetWorld()->SpawnActor<AAGun>(GunClass);
	GetMesh()->HideBoneByName(TEXT("weapon_r"),EPhysBodyOp::PBO_None);
	Gun->AttachToComponent(GetMesh(),FAttachmentTransformRules::KeepRelativeTransform,TEXT("WeaponSocket"));
	Gun->SetOwner(this);
	
}

// Called every frame
void AA_GameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// function for setting up inputs
void AA_GameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"),this,&AA_GameCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveSideward"),this,&AA_GameCharacter::MoveSideward);

	PlayerInputComponent->BindAxis(TEXT("LookUp"),this,&AA_GameCharacter::LookUp);
	PlayerInputComponent->BindAxis(TEXT("LookSides"),this,&AA_GameCharacter::LookSides);

	PlayerInputComponent->BindAction(TEXT("jump"),EInputEvent::IE_Pressed,this,&ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Shoot"),EInputEvent::IE_Pressed,this,&AA_GameCharacter::shoot);
}

//function responsible for moving forward.
void AA_GameCharacter:: MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

//function responsible for moving Sidewards.
void AA_GameCharacter:: MoveSideward(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

//function responsible for lookin up.
void AA_GameCharacter:: LookUp(float AxisValue)
{
	AddControllerPitchInput(AxisValue);
}

//function responsible for looking down.
void AA_GameCharacter:: LookSides(float AxisValue)
{
	AddControllerYawInput(AxisValue);
}

//function for shooting the gun.
void AA_GameCharacter::shoot()
{
	//calling function inside the Gun script.
	Gun->TriggerGun();
}

//function that returns whether the player is still alive.
bool AA_GameCharacter :: IsDead() const
{
	return Health<=0;
}

//Function for dealing with damages
float AA_GameCharacter :: TakeDamage(float DamageValue,struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser)
{
	//constructor
	float DamageApplied = Super::TakeDamage(DamageValue, DamageEvent, EventInstigator, DamageCauser);
	Health-=DamageApplied;
	//if the player dies.
	if(IsDead())
	{
		ATPShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ATPShooterGameModeBase>();
		if(GameMode != nullptr){GameMode->PawnKilled(this);}

		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	return DamageApplied;
}

