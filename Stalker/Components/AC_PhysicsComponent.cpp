#include "AC_PhysicsComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include <Kismet/GameplayStatics.h>
#include "AC_CharacterStats.h"

UAC_PhysicsComponent::UAC_PhysicsComponent()
{
	RootBoneName = "bip01_pelvis";
	PA_ProfileName = "PA_FullBody";

}

void UAC_PhysicsComponent::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* _owner = Cast<ACharacter>(GetOwner());
	Mesh = _owner->GetMesh();
	
	ImpulseWeight = 0.f;

	/*Configure physical animation component*/
	PhysAnimComponent = NewObject<UPhysicalAnimationComponent>(_owner, UPhysicalAnimationComponent::StaticClass(), NAME_None, RF_Transient);
	PhysAnimComponent->RegisterComponent();
	PhysAnimComponent->SetSkeletalMeshComponent(Mesh);
	PhysAnimComponent->ApplyPhysicalAnimationProfileBelow(RootBoneName, PA_ProfileName, false);
}

void UAC_PhysicsComponent::AddImpulse(FName BoneName, FVector Velocity, float ImpulseStrenght, float RecoverInterpolationSpeed)
{
	UAC_CharacterStats* _stats = (UAC_CharacterStats*)GetOwner()->FindComponentByClass<UAC_CharacterStats>();
	
	/*Add recovery only if character alive*/
	if (_stats != nullptr && _stats->IsAlive()) {

		FTimerDelegate recoverImpulse_Delegate;
		recoverImpulse_Delegate.BindUObject(this, &UAC_PhysicsComponent::DecreaseImpulse, RecoverInterpolationSpeed);

		/*Start a recover impulse timer by delegate*/
		GetWorld()->GetTimerManager().SetTimer(ImpulseTimer, recoverImpulse_Delegate, .01f, true);
		ImpulseWeight+=2;
	}
	
	/*Enable mesh physic from the bone, parent to hitted*/
	FName ParentBone = Mesh->GetParentBone(BoneName);
	FName bone = BoneName == RootBoneName?  
		BoneName : ParentBone;
	bool bIncludeSelf = (ParentBone != RootBoneName) && (bone != RootBoneName);

	Mesh->SetAllBodiesBelowSimulatePhysics(bone, true, bIncludeSelf);
		
	/*Check if bone is not root bone*/
	BoneName = BoneName == RootBoneName ? "bip01_spine" : BoneName;

	/*Apply impulse to selected bone*/
	UKismetMathLibrary::Vector_Normalize(Velocity);
	Velocity *= ImpulseStrenght;
	Mesh->AddImpulse(Velocity, BoneName, true);
}

void UAC_PhysicsComponent::DecreaseImpulse(float InterpolationSpeed)
{
	/*Interpolate impulse blend weight every timer tick*/
	ImpulseWeight = UKismetMathLibrary::FInterpTo(ImpulseWeight, 0.f, UGameplayStatics::GetWorldDeltaSeconds(GetWorld()), InterpolationSpeed);

	if (ImpulseWeight <= 0.f) {
		/*Disable physics and clear timer*/
		ImpulseWeight = 0;
		Mesh->SetAllBodiesBelowSimulatePhysics(RootBoneName, false, true);
		GetWorld()->GetTimerManager().ClearTimer(ImpulseTimer);
	}
	else
		/*Apply interpolated impulse blend weight*/
		Mesh->SetAllBodiesBelowPhysicsBlendWeight(RootBoneName, fminf(ImpulseWeight, 1));
}

void UAC_PhysicsComponent::ToRagdoll()
{
	GetWorld()->GetTimerManager().ClearTimer(ImpulseTimer);
	SetStrenghtMultiplyer(0.f);
	Mesh->SetSimulatePhysics(true);
}

/*Overrides Physical animation component function*/
void UAC_PhysicsComponent::SetStrenghtMultiplyer(float InStrenghtMultiplyer)
{
	PhysAnimComponent->SetStrengthMultiplyer(InStrenghtMultiplyer);
}

FName UAC_PhysicsComponent::GetRootBoneName() const{
	return RootBoneName;
}