<template>
  <tr class="payload-info">
    <td>
      <span class="payload-name">{{ payload.name }}</span>
    </td>
    <td class="gauge-col">
      <gauge-bar
          v-if="totalKb"
          :max-value="totalKb"
          :current-value="payload.sizeKb"
          :value-formatter="valueFormatter"
          :border-thickness="2"
          :border-gap="2"
          :bar-height="30">
      </gauge-bar>
    </td>
    <td class="delete-col">
      <span class="btn-delete" @click="deleteMe">×</span>
    </td>
  </tr>
</template>

<script>
import GaugeBar from './GaugeBar';
export default {
  name: 'payload-info',
  components: {
    [GaugeBar.name]: GaugeBar,
  },
  props: {
    payload: Object,
    totalKb: Number,
    valueFormatter: Function,
    deleteHandler: Function,
  },
  methods: {
    deleteMe() {
      if (confirm(`Delete payload ${this.payload.name} \r\nAre you sure?`)) {
        this.deleteHandler(this.payload.key);
      }
    },
  },
};
</script>

<style lang="less">
@import '../variables';

.payload-info {
  cursor: default;
  transition: @hover-transition;

  &:hover {
    background: @primary-colour;
    color: #fff;
    td {
      .btn-delete {
        color: #fff;
      }
    }
  }

  td {
    padding: 5px;
    border-bottom: 1px solid;
    &.gauge-col {
      width: 25%;
    }

    &.delete-col {
      width: 3em;
      text-align: center;
    }

    .payload-name {
      padding: 0 20px;
      font-size: 1.5em;
    }

    .btn-delete {
      cursor: pointer;
      text-decoration: none;
      font-size: 3em;
      line-height: 1em;
      color: @primary-colour;
      display: block;

      &:hover {
        color: @secondary-colour;
      }
    }
  }
}
</style>


