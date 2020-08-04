/**
 * Basic You.i RN app
 */
import React, { Component } from 'react';
import { AppRegistry, Image, NativeModules, StyleSheet, Text, View } from 'react-native';
import { DeviceInfo, FormFactor } from '@youi/react-native-youi';

const { NewRelicBridge } = NativeModules;

const NRLogLevel = {
  NONE: 0,
  ERROR: 1,
  WARNING: 2,
  INFO: 4,
  VERBOSE: 8,
};

export default class YiReactApp extends Component {
  constructor(props) {
    super(props);

    //switch(DeviceInfo.getSystemName()) {
      //case 'iOS':
        //NewRelicBridge.startWithApplicationToken('AA216ffa362de5f23adaec2ab8c09d1db36102ea91');
        //break;
      //case 'tvOS':
        //NewRelicBridge.startWithApplicationToken('AA839df3238dbfabfa2325033f3eee665ef3837b98');
        //break;
      //case 'android':
        //NewRelicBridge.startWithApplicationToken('AAe9970cb20fda505e5d6172c2e80d02e6696278b0');
      //default:
       // break;
   // }

    // NOTE: mimic iOS current DFW implementation
    NewRelicBridge.setMaxEventBufferTime(60);
    NewRelicBridge.setMaxEventPoolSize(1000);

    NewRelicBridge.setLogLevels(
      NRLogLevel.ERROR | NRLogLevel.WARNING | NRLogLevel.INFO
    ); // On PROD pass NONE, but we still need to figure out how to get the environment

    NewRelicBridge.recordCustomEvent('LifeCycleEvent', { eventName: 'constructor' });
  }

  componentDidMount() {
    NewRelicBridge.recordCustomEvent('LifeCycleEvent', { eventName: 'componentDidMount' });

    fetch('https://en.wikipedia.org/wiki/New_Relic', {})
      .then(response => {
        NewRelicBridge.recordCustomEvent('NetworkRequest', { eventName: 'fetch', url: response.url, status: response.status });
      })
      .catch(error => {
        NewRelicBridge.recordCustomEvent('NetworkError', { errorMessage: error.message, url: response.url, status: response.status });
      });
  }

  componentWillMount() {
    NewRelicBridge.recordCustomEvent('LifeCycleEvent', { eventName: 'componentWillMount' });
  }

  render() {
    NewRelicBridge.recordCustomEvent('LifeCycleEvent', { eventName: 'render' });

    return (
      <View style={styles.mainContainer}>
        <View style={styles.headerContainer}>
          <View
            style={styles.imageContainer}
            focusable={true}
            accessible={true}
            accessibilityLabel='You i TV logo'
            accessibilityHint='Image in your first app'
            accessibilityRole='image'
          >
            <Image
              style={styles.image}
              source={{ uri: 'res://drawable/default/youi_logo_red.png' }}
            />
          </View>
        </View>
        <View style={styles.bodyContainer} focusable={true} accessible={true}>
          <Text
            style={styles.headlineText}
            accessibilityLabel='Welcome to your first You I React Native app'
          >
            Welcome to your first You.i React Native app!
          </Text>
          <Text
            style={styles.bodyText}
          >
            For more information on where to go next visit
          </Text>
          <Text
            style={styles.bodyText}
            accessibilityLabel='https://developer dot you i dot tv'
          >
            https://developer.youi.tv
          </Text>
        </View>
      </View>
    );
  }
}

const styles = StyleSheet.create({
  mainContainer: {
    backgroundColor: '#e6e7e7',
    flex: 1
  },
  headerContainer: {
    backgroundColor: '#ffffff',
    justifyContent: 'center',
    alignItems: 'center',
    flex: 2
  },
  imageContainer: {
    justifyContent: 'center',
    alignItems: 'center',
    flex: 2
  },
  image: {
    height: FormFactor.isTV ? 225 : 75,
    width: FormFactor.isTV ? 225 : 75,
    resizeMode: 'contain'
  },
  bodyContainer: {
    alignItems: 'center',
    justifyContent: 'center',
    flex: 1
  },
  headlineText: {
    marginBottom: 10,
    color: '#333333',
    textAlign: 'center'
  },
  bodyText: {
    color: '#333333',
    textAlign: 'center'
  }
});

AppRegistry.registerComponent('YiReactApp', () => YiReactApp);
