//#include "SpaceShip.h"
//#include "Game.h"
//
//using namespace DirectX;
//using namespace SimpleMath;
//
//SpaceShip::SpaceShip(Game* game) : CelestialBody(game, []() {
//    CelestialBodyDesc desc;
//    desc.Radius = 1.0f; // Custom radius
//    desc.SpinSpeed = 0.0f; // Custom spin speed
//    desc.OrbitAnchor = nullptr; // Custom orbit anchor (nullptr in this case)
//    //desc.OrbitOrientationForward = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f); // Custom forward orientation
//    //desc.OrbitOrientationUp = DirectX::SimpleMath::Vector3(0.0f, 1.0f, 0.0f); // Custom up orientation
//    desc.OrbitRadius = 100.0f; // Custom orbit radius
//    desc.OrbitSpeed = 10.0f; // Custom orbit speed
//    desc.PrimaryColor = DirectX::SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f); // Custom primary color (red)
//    desc.SecondaryColor = DirectX::SimpleMath::Vector4(1.0f, 0.0f, 0.0f, 1.0f); // Custom secondary color (blue)
//    return desc;
//    }()) {
//    // Additional SpaceShip initialization code can go here
//    points_.clear();
//    indices_.clear();
//
//    // Apex (tip) of the cone
//    float radius = 1.0f;
//    float height = 4.0f;
//    int sliceCount = 32;
//    Vector4 col1 = Vector4(183.f / 255.f, 183.f / 255.f, 183.f / 255.f, 1.f);
//    Vector4 col2 = Vector4(153.f / 255.f, 65.f / 255.f, 99.f / 255.f, 1.f);
//
//    Point apex({ DirectX::SimpleMath::Vector4(0.0f, height / 2.0f, 0.0f, 1.0f), col1 });
//    points_.push_back(apex);
//
//    // Base vertices
//    const float thetaStep = DirectX::XM_2PI / static_cast<float>(sliceCount);
//    for (int i = 0; i <= sliceCount; ++i) {
//        const float theta = static_cast<float>(i) * thetaStep;
//        Point p;
//        p.pos.x = radius * cosf(theta);
//        p.pos.y = -height / 2.0f; // Base is at y = -height/2
//        p.pos.z = radius * sinf(theta);
//        p.pos.w = 1.0f;
//        Vector4::Lerp(col1, col2, static_cast<float>(i) / sliceCount, p.col); // Interpolate color
//        points_.push_back(p);
//    }
//
//    // Center of the base (for proper shading or if needed)
//    Point baseCenter({ DirectX::SimpleMath::Vector4(0.0f, -height / 2.0f, 0.0f, 1.0f), col2 });
//    points_.push_back(baseCenter);
//
//    // Indices for the sides of the cone (triangles connecting apex to base)
//    for (int i = 0; i < sliceCount; ++i) {
//        indices_.push_back(0); // Apex
//        indices_.push_back(i + 1);
//        indices_.push_back(i + 2);
//    }
//
//    // Indices for the base of the cone (triangles connecting base vertices to the center)
//    const int baseCenterIndex = points_.size() - 1;
//    for (int i = 1; i <= sliceCount; ++i) {
//        indices_.push_back(baseCenterIndex);
//        indices_.push_back(i);
//        indices_.push_back(i + 1);
//    }
//    // Add wings (two tetrahedrons on opposite sides of the base)
//    // Wing 1: Attach to base vertices at i=0 and i=1 (theta = 0 and thetaStep)
//    int quarterSlice = sliceCount / 4; // PI/2 position
//    int wingBaseIdx1 = quarterSlice + 1; // First base vertex (theta PI/2)
//    int wingBaseIdx2 = quarterSlice + 2; // Second base vertex (theta PI/2 thetaStep)
//
//    // Define wing apex (point sticking outward alongz)
//    const float wingHeight = radius * 0.75f; // Wing height, adjust as needed
//    Point wing1Apex;
//    wing1Apex.pos = Vector4(radius * cosf(XM_PIDIV2), -height / 2.0f, radius * sinf(XM_PIDIV2) * 1.5f, 1.0f); // Extend along z
//    wing1Apex.col = col2; // Match base color
//    points_.push_back(wing1Apex);
//    int wing1ApexIdx = points_.size() - 1;
//
//    // Define third vertex to form tetrahedron base (slightly offset)
//    Point wing1BaseExtra;
//    wing1Apex.pos = Vector4(radius * cosf(XM_PIDIV2 + thetaStep / 2.0f), -height / 2.0f - wingHeight * 0.5f, radius * sinf(XM_PIDIV2 + thetaStep / 2.0f), 1.0f); // Downward offset
//    wing1BaseExtra.col = col2;
//    points_.push_back(wing1BaseExtra);
//    int wing1BaseExtraIdx = points_.size() - 1;
//
//    // Wing 1 triangles (3 outward faces)
//    indices_.push_back(wingBaseIdx1);     // Base vertex 1
//    indices_.push_back(wingBaseIdx2);     // Base vertex 2
//    indices_.push_back(wing1ApexIdx);     // Apex
//
//    indices_.push_back(wingBaseIdx1);     // Base vertex 1
//    indices_.push_back(wing1ApexIdx);     // Apex
//    indices_.push_back(wing1BaseExtraIdx); // Extra base vertex
//
//    indices_.push_back(wingBaseIdx2);     // Base vertex 2
//    indices_.push_back(wing1ApexIdx);     // Apex
//    indices_.push_back(wing1BaseExtraIdx); // Extra base vertex
//
//    // Wing 2: Attach to base vertices at theta = 3PI/2 (negative z direction)
//    int threeQuarterSlice = sliceCount * 3 / 4;
//    int wing2BaseIdx1 = threeQuarterSlice + 1; // First base vertex (theta 3PI/2)
//    int wing2BaseIdx2 = threeQuarterSlice + 2; // Second base vertex (theta 3PI/2 thetaStep)
//
//    // Define wing apex (outward along -z)
//    Point wing2Apex;
//    wing2Apex.pos = Vector4(radius * cosf(XM_PI + XM_PIDIV2), -height / 2.0f, radius * sinf(XM_PI + XM_PIDIV2) * 1.5f, 1.0f); // Extend along -z
//    wing2Apex.col = col2;
//    points_.push_back(wing2Apex);
//    int wing2ApexIdx = points_.size() - 1;
//
//    // Define third vertex for tetrahedron base (slightly offset)
//    Point wing2BaseExtra;
//    wing2BaseExtra.pos = Vector4(radius * cosf(XM_PI + XM_PIDIV2 + thetaStep / 2.0f), -height / 2.0f - wingHeight * 0.5f, radius * sinf(XM_PI + XM_PIDIV2 + thetaStep / 2.0f), 1.0f); // Downward offset
//    wing2BaseExtra.col = col2;
//    points_.push_back(wing2BaseExtra);
//    int wing2BaseExtraIdx = points_.size() - 1;
//
//    // Wing 2 triangles (3 outward faces)
//    indices_.push_back(wing2BaseIdx1);     // Base vertex 1
//    indices_.push_back(wing2BaseIdx2);     // Base vertex 2
//    indices_.push_back(wing2ApexIdx);      // Apex
//
//    indices_.push_back(wing2BaseIdx1);     // Base vertex 1
//    indices_.push_back(wing2ApexIdx);      // Apex
//    indices_.push_back(wing2BaseExtraIdx); // Extra base vertex
//
//    indices_.push_back(wing2BaseIdx2);     // Base vertex 2
//    indices_.push_back(wing2ApexIdx);      // Apex
//    indices_.push_back(wing2BaseExtraIdx); // Extra base vertex
//
//}
//
//void SpaceShip::Update()
//{
//    std::cout << absoluteVelocity.x << " " << absoluteVelocity.y << " " << absoluteVelocity.z << std::endl;
//    CelestialBody::Update();
//}
