
declare enum HammerPrimitiveType {
	PNone,
	PSphere,
	PBox,
	PCapsule,
	PEnd
}

declare class HammerFoundation {
	initialize();
	defaultMaterial: HammerMaterial;
}

declare class HammerSceneCreateArgs {
	
}

declare class HammerScene {
	initialize(foundation: HammerFoundation, args: HammerSceneCreateArgs);
}

declare class HammerMaterial {
	constructor(foundation: HammerFoundation,
		staticF: number, dynamicF: number, restitution: number);
}

declare class HammerPrimitiveBody {
	constructor(material: HammerMaterial);
	
	addSphere(transform: Transform, radius: number);
	addBox(transform: Transform, halfExtends: HAnyCoord);
	addCapsule(transform: Transform, radius: number, halfLength: number);
}

declare class HammerPrimitiveHelper {
	static attachPrimitivesToActor(body: HammerPrimitiveBody, node: HammerActorNode);
}

declare class HammerTransformEvent extends HEvent {
	transform: Transform;
}

declare class HammerActorNode extends HNodeImpl {
	constructor(scene: RootNode, transform: Transform);
	
	mass: number;
	invMass: number;
	
	addToScene(scene: HammerScene);
	addForce(force: HAnyCoord);
	addImpulse(impulse: HAnyCoord);
	addAcceleration(acc: HAnyCoord);
	setVelocity(velocity: HAnyCoord);
}
