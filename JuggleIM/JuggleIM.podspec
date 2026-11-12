
Pod::Spec.new do |s|
  s.name             = 'JuggleIM'
  s.version          = '1.10.0'
  s.summary          = 'Juggle IM SDK.'

# This description is used to generate tags and improve search results.
#   * Think: What does it do? Why did you write it? What is the focus?
#   * Try to keep it short, snappy and to the point.
#   * Write the description between the DESC delimiters below.
#   * Finally, don't worry about the indent, CocoaPods strips it!

  s.description      = <<-DESC
Juggle IM SDK for iOS.
                       DESC

  s.homepage         = 'https://juggle.im/'
  s.license          = { :type => "Copyright", :text => "Copyright 2024 Juggle" }
  s.author           = { 'lifei444' => '14392240@qq.com' }
  s.source           = { :http => 'https://downloads.juggle.im/libs/ios/JuggleIM_v1_10_0.zip', :sha256 => '331e662a62bb239bbcac0deab67f6936b5c3a79e6da276b90e5fcf2c3f0b441b' }

  s.ios.deployment_target = '12.0'
  
  s.vendored_frameworks = 'JuggleIM.xcframework'
  # JuggleIM is a static framework. CocoaPods links its code into the app but does not
  # copy files from inside it, so ship the privacy manifest in a resource bundle.
  s.resource_bundles = {
    'JuggleIM_Privacy' => ['JuggleIM.xcframework/ios-arm64/JuggleIM.framework/PrivacyInfo.xcprivacy']
  }
end
