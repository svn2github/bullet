if (CMAKE_SIZEOF_VOID_P MATCHES "8")
SUBDIRS( OpenGL AllBulletDemos  ConvexDecompositionDemo Benchmarks HelloWorld
CcdPhysicsDemo ConstraintDemo SliderConstraintDemo GenericJointDemo
RagdollDemo ForkLiftDemo BasicDemo BspDemo MovingConcaveDemo VehicleDemo
ColladaDemo UserCollisionAlgorithm CharacterDemo SoftDemo TerrainDemo )
else (CMAKE_SIZEOF_VOID_P MATCHES "8")
SUBDIRS( OpenGL AllBulletDemos ConvexDecompositionDemo Benchmarks HelloWorld
MultiThreadedDemo CcdPhysicsDemo ConstraintDemo SliderConstraintDemo
GenericJointDemo RagdollDemo ForkLiftDemo BasicDemo BspDemo MovingConcaveDemo
VehicleDemo ColladaDemo UserCollisionAlgorithm CharacterDemo SoftDemo
TerrainDemo )
endif (CMAKE_SIZEOF_VOID_P MATCHES "8")
